= OpenFlow フレームワーク Trema

#@warn(気の利いたリード文をここに)

第 II 部では、OpenFlow を使ってネットワークをプログラミングする方法を紹
介します。職場や自宅のような中小規模ネットワークでもすぐに試せる実用的
なコードを通じて、「OpenFlow って具体的に何に使えるの？」というよくある
疑問に答えていきます。実例を混じえつつ OpenFlow やネットワークの基礎か
ら説明しますので、ネットワークの専門家はもちろん普通のプログラマもすん
なり理解できるはずです。

まずは、OpenFlow プログラミングのためのフレームワーク Trema (トレマ) を
紹介しましょう。

== Trema とは

Trema は、OpenFlow コントローラを開発するための Ruby および C 用のプロ
グラミングフレームワークです。Trema は github 上で開発されており、
GPLv2 ライセンスのフリーソフトウェアとして公開されています。公開は
2011 年の 4 月と非常に新しいソフトウェアですが、その使いやすさから国内
外の大学や企業および研究機関などですでに採用されています。

Trema の情報は次のサイトから入手できます。

 * Trema ホームページ: @<href>{http://trema.github.com/trema/}
 * github のページ: @<href>{http://github.com/trema/}
 * メーリングリスト: @<href>{http://groups.google.com/group/trema-dev}
 * Twitter アカウント: @<href>{http://twitter.com/trema_news}

===[column] 友太郎の質問: Trema の由来ってなに？

#@warn(友太郎のアイコン)

こんにちは！ 僕は最近OpenFlowに興味を持ったプログラマ，友太郎です。

#@warn(あとで書く)

===[/column]

=== Trema フレームワーク

Trema を使えば、OpenFlow スイッチを持っていなくてもノート PC 1 台で
OpenFlow コントローラの開発とテストができます。これが Trema の「フレー
ムワーク」たるゆえんで、最も強力な機能です。第 2 部では、実際に Trema
を使っていろいろと実験しながら OpenFlow コントローラを作っていきます。

=== セットアップ

Trema は Linux 上で動作し、Ubuntu 10.04 以降および Debian GNU/Linux
6.0 の 32 ビットおよび 64 ビット版での動作が保証されています。テストは
されていませんが、その他の Linux ディストリビューションでも基本的には動
作するはずです。

Trema の提供する trema コマンドの実行には root 権限が必要です。まずは、
@<tt>{sudo} を使って root 権限でコマンドを実行できるかどうか、
@<tt>{sudo} の設定ファイルを確認してください。

//cmd{
% sudo visudo
//}

@<tt>{sudo} ができることを確認したら、Trema が必要とする @<tt>{gcc} な
どの外部ソフトウェアを次のようにインストールします。

//cmd{
% sudo apt-get install git gcc make ruby ruby-dev irb libpcap-dev libsqlite3-dev
//}

次に Trema 本体をダウンロードします。Trema は github 上で公開されており、
@<tt>{git} を使って最新版が取得できます。

//cmd{
% git clone git://github.com/trema/trema.git
//}

Trema のセットアップには、@<tt>{make install} のようなシステム全体へイ
ンストールする手順は不要です。ビルドするだけで使い始めることができます。
ビルドは次のコマンドを実行するだけです。

//cmd{
% ./trema/build.rb
//}

それでは早速、入門の定番 Hello, Trema! コントローラを Ruby で書いてみま
しょう。なお、第 2 部では Trema の Ruby ライブラリを使ったプログラミン
グを取り上げます。C ライブラリを使ったプログラミングの例については、
Trema の @<tt>{src/examples/} ディレクトリ以下を参照してください。第
II 部で使った Ruby コードに加えて、同じ内容の C コードを見つけることが
できます。

== Hello, Trema!

trema ディレクトリの中に @<tt>{hello-trema.rb} というファイルを作成し、
エディタで@<list>{hello-trema.rb}のコードを入力してください。

#@warn(ファイル名は hello-trema.rb だけどクラス名は HelloController?)
//list[hello-trema.rb][Hello Trema! コントローラのソースコード (@<tt>{hello-trema.rb})]{
  class HelloController < Controller
    def start
      puts "Hello, Trema!"
    end
  end
//}

どうでしょうか? とてもシンプルに書けますね。それではこれを実行する前に
即席で Ruby の文法を頭に入れておきましょう。難しそう？朝、同じ車両にル
ビイストが 2 人も乗ってたの気付きましたか。Ruby のプログラマは世界中に
何百万人もいるんです。

== 即席 Ruby 入門

Ruby の品詞には、たいがい見分けるための手がかりとなる視覚的なヒントがあ
ります。区切り記号が使われていたり、最初の文字が大文字になっていたりす
るので、断片的なコードを見ても、あなたの頭は見覚えがあるように強く感じ
るでしょう。あまたの心はしょっちゅう叫びます。「おや、こいつのことなら
知っているぞ!」あなたは他のルビイストたちと話をするときに、そういう「有
名人」たちの名前を挙げて自慢することだってできます。

それぞれの品詞の見かけに注意してください。それぞれの品詞について簡単に
説明しますが、別に説明が全部理解できなくとも構いません。この節の終わる
頃には Ruby プログラムのあらゆる部分が識別できるようになっているはずで
す。

=== キーワード

Rubyにはたくさんの組み込みの語があり、意味が与えられています。これらの
語を変数として使ったり、自分の目的に合わせて意味を変えたりはできません。
こいつらは隠れ家の中にいて、ちょっかいを出すと公式のシンタックスエラー
でもてなされることになります。

//quote{
  alias   and     BEGIN   begin   break   case    class   def     defined 
  do      else    elsif   END     end     ensure  false   for     if 
  in      module  next    nil     not     or      redo    rescue  retry 
  return  self    super   then    true    undef   unless  until   when 
  while   yield 
//}

=== 変数

=== 定数

定数は変数のような語ですが、定数の場合は大文字で始めます。変数が Ruby
の名詞だとすると、定数は固有名詞に当たります。Time や Array や
Bunny_Lake_is_Missing (バニー・レークは行方不明) が例です。英語では、固
有名詞は大文字で始めることになっています。The Empire State Building (エ
ンパイアステートビル) みたいに。エンパイアステートビルというのは動かす
ことができません。エンパイアステートビルを何か別なもののことだと勝手に
決めることもできません。固有名詞というのはそういうものです。固有名詞は
ある特定の、通常は時間とともに変化しないものを指しています。固有名詞と
同様、Rubyの定数は一度セットすると変更できません。

EmpireStateBuilding = "ニューヨーク州ニューヨーク市5番街350"

定数の値を変えようとすると、Rubyは文句を言ってきます。そういうことには難
色を示すのです。

=== 実行してみよう

それでは早速実行してみましょう! 作成したコントローラは @<tt>{trema run} 
コマンドで実行できます。この世界一短いOpenFlowコントローラ(？)は画
面に @<tt>{Hello, Trema!} と出力します。

//cmd{
% cd trema
% ./trema run ./hello-trema.rb
Hello, Trema!  # Ctrl+c で終了
//}

いかがでしょうか？ Trema を使うと、とても簡単にコントローラを書いて実行
できることがわかると思います。えっ？ これがいったいスイッチの何を制御し
たかって？ 確かにこのコントローラはほとんど何もしてくれませんが、Trema
でコントローラを書くのに必要な知識がひととおり含まれています。スイッチ
をつなげるのはちょっと辛抱して、まずはソースコードを見ていきましょう。

=== コントローラを定義する

Ruby で書く場合、すべてのコントローラは @<tt>{Controller} クラスを継承
します (@<list>{クラス定義}の 1 行目)。

//listnum[クラス定義][コントローラのクラスを定義する]{
 @<ami>{class HelloController < Controller}
    def start
      puts "Hello, Trema!"
    end
  end
//}

このように @<tt>{Controller} クラスを継承することで、コントローラに必要
な基本機能が @<tt>{HelloController} クラスにこっそりと追加されます。

=== ハンドラを追加する

Trema はイベントドリブンなプログラミングモデルを採用しています。つまり、
OpenFlow メッセージの到着など各種イベントに対応するハンドラを定義してお
くと、イベントの発生時に対応するハンドラが呼び出されます。たとえば
@<tt>{start} メソッド (@<list>{ハンドラ定義}の 2〜4 行目) を定義しておくと、
コントローラの起動時にこれが自動的に呼ばれます。

//listnum[ハンドラ定義][起動時に呼ばれるハンドラを定義する]{
  class HelloController < Controller
    @<ami>{def start}
      @<ami>{puts "Hello, Trema!"}
    @<ami>{end}
  end
//}  

== Trema のファイル構成

先に進む前に、まずは簡単に Trema のファイル構成を見ておきましょう。
Trema をダウンロードすると、いくつかのファイルとディレクトリがあること
がわかります。次に主要なものを挙げましょう。

 * @<tt>{build.rb}: ビルドスクリプト。
 * @<tt>{cruise.rb}: すべてのテストコードを実行するテストスイート。Trema 開発者向け。
 * @<tt>{features/}: 受け入れテスト一式。Trema 開発者向け。
 * @<tt>{ruby/}: Ruby ライブラリのソースコード。
 * @<tt>{spec/}: Ruby のユニットテスト一式。Trema 開発者向け。
 * @<tt>{src/examples/}: サンプルアプリ。
 * @<tt>{src/lib/}: C ライブラリのソースコード。
 * @<tt>{tmp}: ログファイルや PID ファイルといった一時ファイルの置き場。
 * @<tt>{trema}: trema コマンド本体。
 * @<tt>{unittests/}: C のユニットテスト一式。Trema 開発者向け。
 
この中でも Trema で何か作りたい人が真っ先に読むべきは、サンプルアプリ
(@<tt>{[trema]/src/examples}) です。

===[column] 取間先生曰く: Trema のテスト

Trema にはずいぶんたくさんのテストコードが付いていて、Trema 開発者がテ
ストをとても重視していることがわかるじゃろう。テストの実行頻度も徹底し
ていて、開発者が新しいコードをコミットする度にすべてのテスト
(@<tt>{cruise.rb} スクリプト) を自動的に実行し、「いつダウンロードして
も正しく動く」ことを保証しているのじゃ。これを難しい言葉で "継続的イン
テグレーション" と呼ぶ。

#@warn(テストランプと天井の蛍光灯がかぶって見づらいので、写真を撮り直し)
//image[ccrb][テストの実行結果を示すランプ]

Trema を壊さないために、ひとつおもしろい工夫があるのじゃ。@<img>{ccrb}
はTrema 開発者の机に置いてあるランプで、テストの実行結果をランプの色で
視覚的にフィードバックする。テストがすべて通るとランプが緑色に光る。も
しエラーが起こった場合には、ランプが赤く光り開発メンバー全員にメールが
飛ぶ。これによって、万が一壊してしまっても必ず誰かが気付けるようにして
いるのじゃ。

このしくみには、環境構築が手軽な CruiseControl.rb
(@<tt>{http://cruisecontrolrb.thoughtworks.com/}) と自作プラグインを使っ
ているぞ。

===[/column]

== サンプルアプリ

サンプルアプリ (@<tt>{[trema]/src/examples/}) は簡単な OpenFlow アプリ
ケーションをたくさん含んでおり、実際の API の使い方を調べるのに便利です。
以下におもなサンプルアプリをまとめます (括弧内は
@<tt>{[trema]/src/examples/} 内のディレクトリ名)。簡単な順に上から並べ
ていますので、この順に読んでいくことをおすすめします。またこのうちいく
つかは続く章で詳しく説明していきます。

: こんにちは Trema (@<tt>{hello_trema})
  この第 II 部で説明する Trema プログラミングのすべての基本となる
  "Hello Trema!" と表示するだけのサンプルです。これを @<tt>{trema run}
  コマンドで実行すれば、手っ取り早く Trema を試すことができます。Trema
  を始めたばかりの初心者向け。

: Packet In (@<tt>{packet_in})
  OpenFlow メッセージの中でも重要な Packet In メッセージをハンドルする
  サンプルです。OpenFlow メッセージハンドラの定義方法や、Packet In メッ
  セージの取り扱いの基本が学べます。

: スイッチの監視 (@<tt>{switch_monitor})
  スイッチがコントローラへ接続したり逆に切断したときのイベントを捕捉す
  るサンプルです。複数のハンドラを使った少し複雑なコントローラの実装が
  学べます。@<chap>{switch_monitoring_tool} で詳しく説明します。

: OpenFlow メッセージのダンプ (@<tt>{dumper})
  コントローラが受け取るすべての OpenFlow メッセージを文字列としてダン
  プするサンプルです。さまざまな OpenFlow メッセージのハンドラの書き方
  のリファレンスとして役に立ちます。

: スイッチ情報 (@<tt>{switch_info})
  スイッチの詳細情報を要求する Features Request メッセージをコントロー
  ラに送信し、スイッチから受信したスイッチ情報を出力するサンプルです。
  コントローラからスイッチへ OpenFlow メッセージを送る方法が学べます。

: リピータハブ (@<tt>{repeater_hub})
  いわゆるバカハブ (ダムハブ) の実装です。重要な OpenFlow メッセージで
  ある Flow Mod と Packet Out の基本が学べます。少し進んだ話題として、
  @<chap>{tdd} ではこれを題材にコントローラのテスト駆動開発手法を学びま
  す。

: ラーニングスイッチ (@<tt>{learning_switch})
  普通のスイッチ (レイヤ 2 スイッチ) をエミュレートするサンプルです。
  FDB などスイッチの基本構成を学ぶことができます。詳しくは
  @<chap>{learning_switch} で説明します。

: トラフィックモニタ (@<tt>{traffic_monitor})
  ラーニングスイッチを拡張し、ユーザごとのトラフィックを測れるようにし
  たものです。フローに含まれる統計情報の利用例として役に立ちます。詳し
  くは @<chap>{traffic_monitor} で説明します。

: 複数スイッチ対応ラーニングスイッチ (@<tt>{multi_learning_switch})
  ラーニングスイッチの複数スイッチ版です。ラーニングスイッチとの違い、
  とくにスイッチごとに FDB を管理する部分に注目してください。

Trema にはたくさんの API があり、この表に掲載したサンプルではまだまだす
べてを紹介しきれていません。新しいサンプルアプリを作った人は、ぜひ
github で pull リクエストを送ってください。あなたの名前が Trema プロジェ
クトの貢献者リスト
(@<tt>{https://github.com/trema/trema/graphs/contributors}) に載るかも
しれません!

== まとめ

さて、これで Trema の基本はおしまいです。この章ではすべてのコントローラ
のテンプレートとなる Hello, Trema! コントローラを書きました。学んだこと
は次の 2 つです。

 * コントローラは Ruby のクラスとして定義し、@<tt>{Controller} クラスを
   継承することで必要なメソッドが取り込まれる。
 * コントローラクラスに各種イベントに対応するハンドラを定義することでロ
   ジックを実装できる。たとえば、起動イベントに対応するハンドラは
   start である。
 * Trema のファイル構成をおおざっぱに見ました。続く章では Trema のサン
   プルアプリ (@<tt>{[trema]/src/examples}) のいくつかを題材に、
   OpenFlow プログラミングを学んでいきます。

次の章では、いよいよ実用的な OpenFlow コントローラを書き実際にスイッチ
をつないでみます。

== 参考文献
