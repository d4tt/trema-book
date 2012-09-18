= OpenFlow の仕組み

//lead{
概念が分かったところでもう少し細かい仕様に進みましょう。ここでは実用重視で OpenFlow 仕様のポイントとよく使う用語を押さえます。
//}

OpenFlow は早くから標準化を重視してきました。2008 年にスタンフォード大学を中心とする OpenFlow スイッチコンソーシアムが標準化を開始し、バージョン 1.0 と 1.1 の仕様をリリースしました。その後、2011 年に Google や Facebook などのクラウド系企業が中心となり Software Defined Network 技術の推進を目的とした非営利団体 Open Networking Foundation (ONF) を設立し、それ以降の標準化を行っています。

この OpenFlow 標準仕様が主に定義するのは次の 2 つです。

 1. コントローラとスイッチの間の通信プロトコル (セキュアチャンネル)
 2. 設定されたフローに対するスイッチの振る舞い

//noindent
本章ではこの 2 つの中でも特によく使う機能のみを重点的に説明します。

執筆時の OpenFlow 最新バージョンは 1.3 ですが、以降の説明では対応する商用スイッチも出そろっているバージョン 1.0 をベースに説明します。なお、1.0 とそれ以降では考え方やモデルに大きな変更はありませんので、1.0 を理解していれば新しいバージョンも理解しやすいでしょう。

== スイッチとコントローラ間のやりとり

OpenFlow スイッチとコントローラは OpenFlow 仕様で規定されたメッセージをやりとりしながら動作します。ここでは具体的にどのようなメッセージを送受信するかを順を追って説明します。@<chap>{whats_openflow}で見たカスタマーサービスセンターでのやりとりを思い出しながら読んでください。

=== スイッチとコントローラ間の接続を確立する

最初にコントローラとスイッチはセキュアチャンネルの接続を確立します。OpenFlow にはセキュアチャンネルをスイッチとコントローラのどちらが始めるかの規定はありませんが、現状では@<img>{secure_channel} のようにスイッチがコントローラに接続する実装が多いようです。

//image[secure_channel][スイッチがコントローラに接続しセキュアチャンネルを確立]

セキュアチャンネルは普通の TCP 接続です。コントローラとスイッチの両方が対応している場合には、性能は落ちますがよりセキュアな TLS (Transport Layer Security) を使うこともできます。

=== バージョンのネゴシエーション

次に使用する OpenFlow プロトコルのバージョンを確認するステップ、いわゆるバージョンネゴシエーションが始まります。セキュアチャンネルを確立すると、スイッチとコントローラはお互いに自分のしゃべれるバージョン番号を乗せた Hello メッセージを出し合います (@<img>{version_negotiation})。

//image[version_negotiation][Hello メッセージを出し合うことで相手の OpenFlow プロトコルバージョンを確認]

相手と同じバージョンを話せるようであればネゴシエーションに成功で、本格的におしゃべりを始められるようになります。

=== スイッチのスペックの確認

次にコントローラは接続したスイッチがどんなスイッチかを確認します。ネゴシエーション直後はまだバージョンしか確認できていないので、コントローラはスイッチに Features Request というメッセージを送って次の各情報をリクエストします。

 * スイッチのユニーク ID (Datapath ID)
 * 物理ポートの一覧情報
 * サポートする機能の一覧

//image[features_request_reply][Features Request メッセージでスイッチのスペックを確認]

スイッチは Features Reply メッセージでこの情報を返信します。

=== 未知のパケットの受信

スイッチはフローとして登録されていない通信を検知すると、コントローラにそのパケットを通知します。これは Packet In メッセージと呼ばれます。

//image[packet_in][未知のパケットとその情報が Packet In メッセージとしてコントローラに上がる]

=== フローテーブルの更新とパケットの転送

パケットを転送する場合には、まず Packet Out という OpenFlow メッセージによって Packet In を起こしたパケット自体を正しい宛先に流してやります。これをやらないと、Packet In を起こしたパケットがコントローラに残ったままになってしまうからです。そして、同様の通信を今後はスイッチ側だけで処理するために、Flow Mod メッセージによってフローをスイッチのフローテーブルに書き込みます。

//image[flowmod_packetout][Flow Mod によってフローテーブルを更新し、Packet In を起こしたパケットを Packet Out で転送]

=== フローの寿命と統計情報

Flow Mod で打ち込むフローには「寿命」を設定できます。寿命の指定には次の 2 種類があります。

 * アイドルタイムアウト: フローが参照されない時間がこの寿命に逹するとフローを消す。パケットが到着し、フローが参照された時点で 0 秒にリセットされる。
 * ハードタイムアウト: フローの参照の有無を問わず、フローが書き込まれてからの時間がこの寿命に逹するとフローを消す。

どちらのタイムアウトも 0 にしてフローを打ち込むと、そのフローは明示的に消さない限りフローテーブルに残ります。

フローテーブルからフローが消されるとき、消されたフローの情報とフローにしたがって処理されたパケットの統計情報がコントローラに通知されます。これを Flow Removed メッセージと呼びます。このメッセージはネットワークのトラフィック量の集計に使えます。

//image[flow_removed][フローが寿命で削除されると、フローによって転送されたパケットの統計情報が Flow Removed としてコントローラに上がる]

== フローの中身

@<chap>{whats_openflow}で見たようにフローは次の 3 要素から成ります。

 * マッチングルール
 * アクション
 * 統計情報
 
以下ではそれぞれの中身を少し細かく見ていきます。なお、これらを最初からすべて頭に入れる必要はありません。以降の章を読んでいてわからなくなったらレファレンスとして活用してください。

=== マッチングルール

マッチングルールとは、OpenFlow スイッチがパケットを受け取ったときにアクションを起こすかどうかを決める条件です。たとえば「パケットの宛先が http サーバーだったら」とか「パケットの宛先がブロードキャストアドレスだったら」などという条件に適合したパケットにだけ、スイッチがアクションを起こすというわけです。

OpenFlow 1.0 では、@<table>{matching_rules} の 12 種類の条件が使えます。これらの条件はイーサネットや TCP/UDP でよく使われる値です。

===[column] 取間先生曰く: マッチングルールの別名

フローの 3 要素のひとつ、マッチングルールには実は "OpenFlow 12 タプル"、"ヘッダフィールド" という別の呼び方もあって、みんな混乱しとるんじゃ。そこでこの本では" マッチングルール" で統一することにした。パケットが来たときにルールに従ってマッチする、という役割をすなおに表現していて、いちばんわかりやすい名前だからじゃ。

"OpenFlow 12 タプル" は OpenFlow 1.1 では 15 タプルになるし、この先どこまで増えるかわからん。つまりいつまで使えるかわからん名前じゃ。また "ヘッダフィールド" じゃ、何をするものか名前からはわかりづらいし、変に難しそうじゃろ。

===[/column]

//table[matching_rules][マッチングルールで指定できる 12 種類の条件]{
名前				説明				
--------------------------------------------------------------
Ingress Port		スイッチの物理ポート番号
Ether src			送信元 MAC アドレス
Ether dst			宛先 MAC アドレス
Ether type			イーサネットの種別
IP src				送信元 IP アドレス
IP dst				宛先 IP アドレス
IP proto			IP のプロトコル種別
IP ToS bits			IP の ToS 情報
TCP/UDP src port	TCP/UDP の送信元ポート番号
TCP/UDP dst port	TCP/UDP の宛先ポート番号
VLAN id				VLAN ID
VLAN priority		VLAN PCP の値 (CoS)
//}

OpenFlow の世界では、このマッチングルールで指定できる条件を自由に組み合わせて通信を制御します。たとえば、

 * スイッチの物理ポート 1 番から届く、宛先が TCP 80 番 (= HTTP) のパケットを書き換える
 * MAC アドレスが 02:27:e4:fd:a3:5d で宛先の IP アドレスが 192.168.0.0/24 は遮断する

//noindent
などといった具合です。

もちろん、12 種類の条件すべてを指定したマッチングルールを作ることもできます。これを Exact Match と呼びます。対して上の例のように一部の条件だけを指定したものを Wildcard Match と呼びます。特定のパケットだけをピンポイントでフローにひっかける場合は Exact Match を使い、幅広い種類のパケットをひとつのフローでひっかける場合には Wildcard Match を使うと覚えてください。

===[column] 取間先生いわく: OSI ネットワークモデルが壊れる？

あるネットワークの経験豊富な若者がこんな事を言っていた「OpenFlow のようにレイヤをまたがって自由に何でもできるようになると、OSI ネットワークモデル (よく「レイヤ 2」とか「レイヤ 3」とか呼ばれるアレのこと。正確には ISO によって制定された、異機種間のデータ通信を実現するためのネットワーク構造の設計方針。) が壊れるんじゃないか？」

その心配は無用じゃ。OSI ネットワークモデルは正確に言うと「OSI 参照モデル」と言って、通信プロトコルを分類して見通しを良くするために定義した "参照用" の階層モデルじゃ。たとえば自分が xyz プロトコルというのを作ったけど人に説明したいというときに、どう説明するか考えてみてほしい。「これはレイヤ 3 のプロトコルで、…」という風に階層を指して (参照して) 説明を始めれば相手に通りがいいじゃろ。つまり、OSI ネットワークモデルはネットワーク屋同士で通じる「語彙」として使える、まことに便利なものじゃ。

でも、これはあくまで「参照」であって「規約」ではないので、すべてのネットワークプロトコル、ネットワーク機器がこれに従わなきゃならん、というものではないぞ。さっき言ったように「この ○○ は、仮に OSI で言うとレイヤ4 にあたる」のように使うのが正しいのじゃ。

そして、OpenFlow はたまたまいくつものレイヤの情報が使える、ただそれだけのことじゃ。

===[/column]

=== アクション

アクションとは、スイッチに入ってきたパケットをどう料理するか、という@<em>{動詞}にあたる部分です。よく「OpenFlow でパケットを書き換えて曲げる」などと言いますが、こうした書き換えなどはすべてアクションで実現できます。それでは、OpenFlow 1.0 ではどんなアクションが定義されているか見てみましょう。

==== アクションの種類

アクションは大きく分けて次の 4 種類があります。

 * Forward: パケットを指定した宛先ポートに出す
 * Modify-Field: パケットの中身を書き換える
 * Drop: パケットを捨てる
 * Enqueue: ポートごとに指定されたスイッチのキューに入れる。QoS 用。
 
アクションは動詞と同じく指定した順番に実行されます。「おにぎりを作って、食べて、片付ける」といったふうに。たとえば、パケットを書き換えて指定した宛先ポートに出したいときには、

//emlist{
[Modify-Field, Forward]
//}

というアクションのリストを指定します。ここで、アクションは指定された順番に実行されることに注意してください。アクションの順番を変えてしまうと、違う結果が起こります。たとえば「おにぎりを食べてから、おにぎりを作る」と最後におにぎりが残ってしまいます。同様に先ほどの例を逆にしてしまうと、まず先にパケットがフォワードされてしまいます。その後 Modify-Field が実行されても、書き換えられた後、そのパケットは破棄されるだけです。

//emlist{
# パケットを書き換える前にフォワードされてしまう。
[Forward, Modify-Field]
//}

同じ動詞を複数指定することもできます。

//emlist{
[Modify-Field A, Modify-Field B, Forward A, Forward B]
//}

この場合は、フィールド A と B を書き換えて、宛先ポート A と B へフォワードする、と読めます。このように、複数のフィールドを書き換えたり複数の宛先ポートにパケットを出したい場合には、アクションを複数連ねて指定します@<fn>{num_actions}。

//footnote[num_actions][指定できるアクション数の上限は OpenFlow スイッチとコントローラの実装に依存します。普通に使う分にはまず問題は起こらないでしょう。]

Drop は特殊なアクションで、実際に Drop アクションというものが具体的に定義されているわけではありません。アクションのリストに Forward アクションをひとつも入れなかった場合、そのパケットはどこにもフォワードされずに捨てられます。これを便宜的に Drop アクションと呼んでいるわけです。

それでは、最もよく使われる Forward アクションと Modify-Field アクションで具体的に何が指定できるか見て行きましょう。

===== Forward アクション

Forward アクションでは指定した宛先ポートにパケットを転送します。宛先ポートはポート番号で指定することもできますが、いくつかの論理的なポートが定義されています。

 * ポート番号: パケットを指定した番号のポートに出す。
 * IN_PORT: パケットを入ってきたポートに出す。
 * ALL: パケットを入ってきたポート以外のすべてのポートに出す。
 * FLOOD: パケットをスイッチが作るスパニングツリーに沿って出す。
 * CONTROLLER: パケットをコントローラに明示的に送り、Packet In を起こす。
 * NORMAL: パケットをスイッチの機能を使って転送する。
 * LOCAL: パケットをスイッチのローカルスタックに上げる。ローカルスタック上で動作するアプリケーションにパケットを渡したい場合に使う。あまり使われない。

この中でも FLOOD や NORMAL は OpenFlow スイッチ機能とスイッチ本来の機能を組み合わせて使うための論理ポートです。

===== Modify-Field アクション

Modify-Field アクションではパケットのさまざまな部分を書き換えできます。

 * 送信元 MAC アドレスの書き換え: 送信元の MAC アドレスを書き換える。
 * 宛先 MAC アドレスの書き換え: 宛先の MAC アドレスを書き換える。
 * 送信元 IP アドレスの書き換え: 送信元の IP アドレスを書き換える。
 * 宛先 IP アドレスの書き換え: 宛先の IP アドレスを書き換える。
 * ToS フィールドの書き換え: IP の ToS フィールドを書き換える。
 * TCP/UDP 送信元ポートの書き換え: TCP/UDP の送信元ポート番号を書き換える。
 * TCP/UDP 宛先ポートの書き換え: TCP/UDP の宛先ポート番号を書き換える。
 * VLAN ヘッダの除去: VLAN のヘッダを除去する。
 * VLAN ID の書き換え: 指定された VLAN ID をセットする、または既存のものがあれば書き換える。
 * VLAN プライオリティの書き換え: 指定された VLAN プライオリティをセットする、または既存のものがあれば書き換える。

それぞれのアクションでできることと、代表的な使い道を順番に見ていきましょう。

====== MAC アドレスの書き換え

MAC アドレス書き換えの代表的な例がルーターです。OpenFlow はルーターの実装に必要な、送信元と宛先 MAC アドレスの書き換えアクションをサポートしています。

//image[rewrite_mac][ルーターでの送信元と宛先 MAC アドレスの書き換え][scale=0.3]

ルーターは 2 つのネットワークの間で動作し、ネットワーク間で行き交うパケットの交通整理を行います。ホスト A が異なるネットワークに属するホスト B にパケットを送ると、ルーターはそのパケットを受け取りその宛先 IP アドレスから転送先のネットワークを決定します。そして、パケットに記述された宛先 MAC アドレスを次に送るべきホストの MAC アドレスに、送信元を自分の MAC アドレスに書き換えてデータを転送します。

====== IP アドレスの書き換え

IP アドレス書き換えの代表的な例が NAT (Network Address Transition) です。OpenFlow は NAT の実装に必要な、送信元と宛先 IP アドレスの書き換えアクションをサポートしています。

//image[rewrite_ip_address][NAT での送信元と宛先 IP アドレスの書き換え][scale=0.3]

インターネットと接続するルーターでは、プライベート/グローバルネットワーク間での通信を通すために IP アドレスを次のように変換します。プライベートネットワーク内のクライアントからインターネット上のサーバーに通信をする場合、ゲートウェイはプライベートネットワークから届いたパケットの送信元 IP アドレスを自分のグローバルな IP アドレスに変換して送信します。逆にサーバーからの返信は逆の書き換えを行うことによりプライベートネットワーク内のクライアントに届けます。

====== ToS フィールドの書き換え

ToS フィールドは通信のサービス品質 (QoS) を制御する目的でパケットを受け取ったルーターに対して処理の優先度を指定するために使われます。OpenFlow はこの ToS フィールドの書き換えアクションをサポートしています。

====== TCP/UDP ポート番号の書き換え

TCP/UDP ポート番号書き換えの代表的な例が IP マスカレードです。OpenFlow は IP マスカレードの実装に必要な、送信元と宛先の TCP/UDP ポート番号の書き換えアクションをサポートしています。

//image[rewrite_port][IP マスカレードでの送信元と宛先 TCP/UDP ポート番号の書き換え][scale=0.3]

ブロードバンドルーターなど 1 つのグローバルアドレスで複数のホストが同時に通信を行う環境では、NAT だけだと TCP/UDP のポート番号が重複する可能性があります。そこで、IP マスカレードではプライベートネットワーク側のポート番号をホストごとに適当に割り当て、通信のつどポート番号を変換することで解決します。

====== VLAN ヘッダの書き換え

既存のタグ付き VLAN で構築したネットワークと OpenFlow で構築したネットワークを接続するという特別な用途のために、VLAN ヘッダの書き換えアクションがあります。VLAN をひとことで説明すると、既存のスイッチで構成されるネットワーク (ブロードキャストが届く範囲のネットワーク) を複数のネットワークに分割して使用するための仕組みです。この分割したネットワーク自体を VLAN と呼ぶ場合もあります。どの VLAN に所属するかを区別するのが VLAN のタグ (VLAN ID) で、パケットに付与される VLAN ヘッダがこのタグ情報を含みます。Modify-Field アクションは VLAN ヘッダの操作に必要なアクションを 3 種類用意しています。

//image[strip_vlan][VLAN ヘッダを書き換えるアクションの使い道][scale=0.6]

: VLAN ヘッダの除去
  VLAN を流れる VLAN ヘッダ付きパケットから VLAN ヘッダを除去し、普通のパケットに戻すアクションです。

: VLAN ID の書き換え
  VLAN パケットが属する仮想 LAN の ID を書き換えます。たとえば VLAN ID を 3 に書き換えるといったアクションを指定できます。また、VLAN ヘッダがついていないパケットに 指定した VLAN ID を持つ VLAN ヘッダを付与することもできます。

: VLAN プライオリティの書き換え
  VLAN 上でのパケットを転送する優先度を変更します。このプライオリティはトラフィックの種類 (データ、音声、動画など) に応じて指定できます。指定できる値は 0 (最低) から 7 (最高) までです。

=== 統計情報

OpenFlow 1.0 ではフローごとに次の統計情報を取得できます。

 * 受信パケット数
 * 受信バイト数
 * フローが作られてからの経過時間 (秒)
 * フローが作られてからの経過時間 (ナノ秒)

== まとめ

OpenFlow 仕様の中でも特にポイントとなる部分を見てきました。@<chap>{whats_openflow}での概要とこの章で学んだポイントが分かれば、あなたはすでに OpenFlow 専門家と言ってよいでしょう。次の章では OpenFlow の典型的な活用例を見ることでさらに OpenFlow を語れるようになりましょう。

