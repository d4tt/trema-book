= トラフィックモニタ

//lead{
ラーニングスイッチの応用がこのトラフィックモニタです。ちょっとしたコードを足すだけで、簡単に機能を追加できる OpenFlow のパワーを実感してください。
//}

@<chap>{learning_switch}で作ったラーニングスイッチをベースに、少し高機能なスイッチを作ってみましょう。トラフィックモニタは，パっと見は普通のラーニングスイッチとして動作します。しかし，裏では各ホストが送信したトラフィックをカウントしており，定期的にこのトラフィック情報を表示します。これを使えば，たとえばネットワークを無駄に使いすぎているホストを簡単に特定できます。

この章でのポイントはずばり、トラフィック情報の収集方法です。これは、@<chap>{openflow_usecases}で紹介したフローでできる 4 つのことの一つです。トラフィック情報をスイッチから取得するには、Flow Mod でフローエントリを書き込むときに寿命 (タイムアウト値) を設定しておき、一定時間後にフローエントリが削除されるようにしておきます。タイムアウトしてフローエントリが削除されると、これを通知する Flow Removed メッセージがコントローラに上がります。この Flow Removed メッセージにはトラフィック情報が含まれているので、コントローラで取り出してやります (@<img>{flow_removed})。

//image[flow_removed][Flow Removed メッセージから統計情報を取得する][scale=0.5]

トラフィック収集以外の部分は、ラーニングスイッチとまったく変わりません。ソースコードの差分も小さいので、前置きはこのくらいにいきなりソースコードを見てみましょう。

== ソースコード

トラフィックモニターはおおまかに「ラーニングスイッチ機能」と「トラフィックの集計機能」の 2 つの機能から成ります。コードを分かりやすくするため、それぞれを独立したクラスとして @<tt>{.rb} ファイルを分けて実装してあります。

 * @<tt>{TrafficMonitor} クラス: ラーニングスイッチにトラフィック収集機能を追加したもの
 * @<tt>{Counter} クラス: 収集したトラフィック情報を集計するクラス

//noindent
このうち @<tt>{TrafficMonitor} クラスがコントローラの本体で、@<tt>{Counter} クラスを使うという関係になります。

=== @<tt>{TrafficMonitor} クラス

まずは @<tt>{TrafficMonitor} クラスのソースコード (@<list>{traffic_monitor_class}) をざっと眺めてみましょう。とくに、ラーニングスイッチとの差分に注目してください。

//list[traffic_monitor_class][本体 @<tt>{TrafficMonitor} クラス（@<tt>{traffic-monitor.rb}）]{
require "counter"


class TrafficMonitor < Controller
  periodic_timer_event :show_counter, 10


  def start
    @fdb = {}
    @counter = Counter.new
  end


  def packet_in( datapath_id, message )
    @fdb[ message.macsa ] = message.in_port
    port_no = @fdb[ message.macda ]
    if port_no
      flow_mod datapath_id, message, port_no
      packet_out datapath_id, message, port_no
    else
      flood datapath_id, message
    end
    @counter.add message.macsa, 1, message.total_len
  end


  def flow_removed( datapath_id, message )
    @counter.add message.match.dl_src, message.packet_count, message.byte_count
  end


  private


  def show_counter
    puts Time.now
    @counter.each_pair do | mac, counter |
      puts "#{ mac } #{ counter[ :packet_count ] } packets (#{ counter[ :byte_count ] } bytes)"
    end
  end


  def flow_mod( datapath_id, message, port_no )
    send_flow_mod_add(
      datapath_id,
      :hard_timeout => 10,
      :match => ExactMatch.from( message ),
      :actions => SendOutPort.new( port_no )
    )
  end


  def packet_out( datapath_id, message, port_no )
    send_packet_out(
      datapath_id,
      :packet_in => message,
      :actions => SendOutPort.new( port_no )
    )
  end


  def flood( datapath_id, message )
    packet_out datapath_id, message, OFPP_FLOOD
  end
end
//}

まず、ラーニングスイッチのコードと比べて、新たに 2 つのメソッドが追加されていることが分かるでしょう。それぞれの処理内容はだいたい次のように読めます。

 1. @<tt>{flow_removed} メソッド: Flow Removed メッセージを受け取るハンドラです。Flow Removed メッセージに含まれるトラフィック情報を取り出してカウンタを更新しています。
 2. @<tt>{show_counter} メソッド: トラフィックの集計情報を表示します。冒頭の @<tt>{periodic_timer_event} により、10 秒ごとに情報を表示します。

そのほかの変更点は、Packet In と Flow Mod の部分でトラフィック集計用にそれぞれ 1 行ずつコードが追加されています。ラーニングスイッチとの差分は、たったこれだけです。

=== @<tt>{Counter} クラス

@<tt>{Counter} クラス（@<list>{counter_class}）はトラフィックモニタのために新たに導入したクラスで、@<tt>{TrafficMonitor} クラスが収集したトラフィック情報を集計します。

//list[counter_class][トラフィックを記録し集計する @<tt>{Counter} クラス（@<tt>{counter.rb}）]{
class Counter
  def initialize
    @db = {}
  end


  def add( mac, packet_count, byte_count )
    @db[ mac ] ||= { :packet_count => 0, :byte_count => 0 }
    @db[ mac ][ :packet_count ] += packet_count
    @db[ mac ][ :byte_count ] += byte_count
  end


  def each_pair( &block )
    @db.each_pair &block
  end
end
//}

このクラスは次の 2 つのパブリックメソッドを持ちます:

 * @<tt>{add} メソッド: 収集したトラフィック情報を記録します。ホストごと (MAC アドレスで区別) に、送信パケット数とバイト数を記録しています。
 * @<tt>{each_pair} メソッド: 集計情報をホストごとに処理するときに使う、一種のループです。ここでは見慣れない文法を使っていますので、詳しくは後の「即席 Ruby 入門」で見ていくことにしましょう。

それでは動作のイメージをつかむため、細かい実装の解説は後回しにしてさっそく実行してみましょう。

== 実行してみよう

今回も仮想ネットワークを使って、仮想スイッチ 1 台、仮想ホスト 2 台の構成でトラフィックモニタを起動してみます。@<list>{traffic_monitor_conf} の内容の仮想ネットワーク設定を @<tt>{traffic-monitor.conf} として保存します。

//list[traffic_monitor_conf][仮想スイッチ 0xabc に仮想ホスト host1，host2 を接続する設定]{
vswitch {
  datapath_id "0xabc"
}

vhost ("host1") {
  ip "192.168.0.1"
  mac "00:00:00:00:00:01"
}

vhost ("host2") {
  ip "192.168.0.2"
  mac "00:00:00:00:00:02"
}

link "0xabc", "host1"
link "0xabc", "host2"
//}

次のように @<tt>{trema run} の "@<tt>{-c}" オプションにこの設定ファイルを渡してトラフィックモニタを実行します。トラフィックモニタはスイッチにつながったホストからのトラフィックを検知し、ホストごとに集計して表示します。ただし起動直後ではまだトラフィックは発生していないので、画面には何も出力されません。

//cmd{
% trema run ./traffic-monitor.rb -c ./traffic-monitor.conf
//}

それでは，実際にトラフィックを発生させて集計されるか見てみましょう。@<tt>{send_packets} コマンドを使って host1 と host2 の間でテストパケットを送ってみます。

//cmd{
% trema send_packets --source host1 --dest host2 --n_pkts 10 --pps 10
% trema send_packets --source host2 --dest host1 --n_pkts 10 --pps 10
//}

@<tt>{trema run} を実行した元のターミナルに、次のような出力が出ていれば成功です。

//cmd{
% trema run ./traffic-monitor.rb -c ./traffic-monitor.conf
……
00:00:00:00:00:01 10 packets (640 bytes)
00:00:00:00:00:02 10 packets (640 bytes)
……
//}

//noindent
host1 と host2 ごとに 10 パケットずつ送信したことが、コントローラからきちんと検知されています。

トラフィックモニタの動作イメージがわかったところで、ソースコードの解説に移りましょう。まずはこの章で新しく登場した Ruby の構文を紹介します。

== 即席 Ruby 入門

== ソースコード

それでは，とくに重要なトラフィック情報収集の処理を詳しく見ていきましょう。メインは Packet In メッセージの受信と Flow Mod をスイッチに打ち込む部分のラーニングスイッチとの違いです。

=== Packet In したパケットをカウントする

フローテーブルに載っていない未知のパケットが Packet In として届くと、コントローラはラーニングスイッチと同様の処理をした後、Packet In したパケット 1 つ分のトラフィック情報をカウンタに記録します。

//image[packet_in][Packet In したパケットもカウンタに記録する][scale=0.5]

//emlist{
def packet_in( datapath_id, message )
  # (ラーニングスイッチと同じ処理)
  # ...

  @counter.add message.macsa, 1, message.total_len
end
//}

//noindent
もしこれをやらないと、収集したトラフィック情報が若干少ない値になってしまうので注意してください。

=== フローエントリのタイムアウトを設定する

Packet In ハンドラの処理として Flow Mod を打つとき、次のように @<tt>{:hard_timeout} オプションを追加で設定していました。

//emlist{
  def flow_mod( datapath_id, message, port_no )
    send_flow_mod_add(
      datapath_id,
      :hard_timeout => 10,
      :match => ExactMatch.from( message ),
      :actions => SendOutPort.new( port_no )
    )
  end
//}

//noindent
このオプションを設定すると、フローエントリが書き込まれてからの秒数がこの値に逹すると強制的にそのフローエントリを消します。つまり、この例では Flow Mod からきっかり 10 秒後に Flow Removed メッセージがコントローラまで上がることになります。今回のようにトラフィック情報を定期的に取得したい場合には、@<tt>{:hard_timeout} を使うと良いでしょう。

タイムアウトにはこの他にもアイドルタイムアウト (@<tt>{:idle_timeout}) というオプションもあります (@<chap>{openflow})。これは、フローエントリが参照されない時間 (アイドル時間) がこの寿命に逹すると消す、というものです。このオプションは主に、一定期間使われていないフローエントリを消すことでフローテーブルの容量を節約するのに使います。

== まとめ

ラーニングスイッチの応用として、「トラフィック集計機能付きスイッチ」を実現するコントローラを書きました。

 * トラフィック情報はフローエントリが消えるときに発生する Flow Removed メッセージの中身を調べることで集計できます。
 * フローエントリの寿命は Flow Mod メッセージのタイムアウトオプションとしてで指定できます。

続く 2 つの章では、第 II 部プログラミング編の締めくくりとしてソフトウェアルータを OpenFlow で作ります。今までに学んできた OpenFlow や Ruby プログラミングの知識を総動員します。