= OpenFlow のユースケース

//lead{
ざっくりと OpenFlow を理解したところで、さっそく実例です。OpenFlow が使えるユースケースとはどんなものでしょうか。
//}

OpenFlow スイッチには「コントローラさえ書けばどんなネットワーク機器にも化けられる」という特長があります。乱暴に言うと、スイッチはもちろんルータやロードバランサ、ファイアウォールや NAT など複雑な機器も、コントローラの実装を頑張れば実現できてしまいます。もちろん、専用機と同じ機能をすべて実装するのは不可能ですし、機能の一部をソフトウェアとして実装することになるので性能は落ちます。しかし、ソフトウェア次第で何でもできる事に変わりはありません。

この何でもできるという特長は、元々は大学や研究所などアカデミアからのニーズによって生まれたものでした。今までのスイッチやルータにとらわれない、まったく新しいインターネットを研究したい。でもすでにあるスイッチやルータのファームウェアを改造するのは大変、かと言って一からハードウェアは作りたくないし…大規模な仮想ネットワーク上で実験してもいいけど、それだと実際のインターネット環境とあまりにも違いすぎる。このジレンマを解消するために考え出されたのが OpenFlow だったわけです。

OpenFlow は、自分の考えた方式をてっとり早く実験できる手段として考えられました。機能面では専用機には及びませんが、ソフトウェアで新機能を追加できるという柔軟性があります。また、パケット転送はハードウェアで行うので性能も出ます。このため、実際のネットワークに接続してトラフィックを流し込むといった本格的な実験も可能です。

== なぜ OpenFlow が流行るのか？

では、インターネットの研究用だった OpenFlow が巨大データセンターに代表される産業界でも注目されるようになったのはなぜでしょうか？

データセンターで重視されるのはスループットとコストです。Google などの巨大データセンターでは、全体のスループットをかせぐためにたくさんのスイッチやサーバをそろえる必要があります。ハイエンドのハードウェアは高くつくため、秋葉原でも買えるごく普通のパーツから構成される安いハードウェアを使います。個々のハードウェアの信頼性は重視されません。Google データセンターのように数十万台のサーバがある環境では、そもそもすべてのハードウェアが故障無しに動くことは期待できないからです。そのかわり、上のソフトウェア層で信頼性を保証します。

OpenFlow はこのデータセンターのモデルにうまくマッチしています。OpenFlow スイッチのハードウェアは従来に比べて単純なので、量産や低コスト化が簡単です。OpenFlow コントローラは汎用サーバ上で動くので、ハイエンドなサーバーは必要ありません。ソフトウェアであるコントローラがネットワークすべてを制御するので、データセンターの制御ソフトウェア層との連携が簡単です。また、データセンターは OpenFlow のような新しい技術を採用しやすいプラットフォームです。インターネットに求められるような相互接続性は重要ではないため、独自の新技術の導入がやりやすいのです。

このように「柔軟性があり既存システムとの連携が簡単」という OpenFlow の設計が、たまたまデータセンターにマッチした、これが OpenFlow が注目される理由です。

== ユースケースあれこれ

データセンターの中で使われる主なネットワーク部品には、スイッチ、ルータ、そしてネットワーク経路の制御があります。今まで見てきたように、データセンターでは必要な機能が実装できればそれでよく、従来のネットワークとの相互接続性は重視されません。このため、OpenFlow で実装するネットワーク部品には最低限必要な機能さえあれば良いことになります。以降では、これらの部品を OpenFlow で実装する方法を説明します。その前に、OpenFlow でできることをおさらいしておきましょう。

「OpenFlow でできることは何か？」という質問を言いかえると、「フローでできることは何か？」になります。昔懐かしい水道管ゲームを思い出してください。水道管のバルブから蛇口まで、さまざまな形の水道管のカードをならべて水を届けるゲームです。フローでできることはこれによく似ています。できることは基本的には次の 4 つです。

 1. パケットを転送する
 2. 流量を調べる
 3. 書き換える
 4. 分岐する

これがすべてです。パケットを指定したスイッチポートから出力することで転送する。フローによって転送されたトラフィック量を調べる。パケットのヘッダを書き換える。パケットを複製して複数のスイッチポートから出力する。これらを自由に組み合わせて、さまざまなタイプのネットワークを作れます。

それではいよいよ実際のユースケースを見ていきましょう。上の 5 種類のカードをどのように組み合わせて実現しているかというところに注目してください。

=== スイッチ

「パケットを転送する」を使うと、最もシンプルなスイッチを OpenFlow で実現できます (@<img>{switch})。スイッチは届いたパケットの宛先の MAC アドレスを見て、その MAC アドレスを持つホストがつながるポートへとパケットを転送します。

//image[switch][OpenFlow でスイッチを実現する][scale=0.5]

これに「流量を調べる」を組み合わせると、スイッチにトラフィック集計機能を付け加えられます。フローごとにどれだけのパケットを転送したかを集計することで、コントローラでネットワーク全体のトラフィックを集計できます。

//image[traffic_switch][OpenFlow でトラフィック集計機能つきスイッチを実現する][scale=0.5]

なお、スイッチの OpenFlow での詳しい実装方法は@<chap>{learning_switch}で、またトラフィック集計機能は@<chap>{traffic_monitor}で説明します。

=== ルータ

「パケットを転送する」に「書き換える」を組み合わせると、ルータを OpenFlow で実現できます (@<img>{router})。ルータは異なる 2 つのネットワークの間で動作し、ネットワーク間のパケットのやりとりに必要な転送と書き換え処理を行います。パケットがルータを通るとき、ルータはパケットの宛先と送信元の MAC アドレスを書き換えて転送します。

//image[router][OpenFlow でルータを実現する][scale=0.5]

なお、ルータの OpenFlow での詳しい実装方法は@<chap>{router_part1}および@<chap>{router_part2}も説明します。

=== 応用例 1: セキュアなネットワークを作る

「パケットを転送する」と「書き換える」、およびホスト認証サーバを組み合わせると、安全な認証済みネットワークを実現できます (@<img>{secure_network})。この認証済みネットワークとは、ホストを認識し認証したホストのみが接続できるネットワークです。コントローラはホストからのパケットが到着すると、そのパケットが認証済ホストからのものかどうかの確認を認証サーバに依頼します。このとき、「書き換える」と「パケットを転送する」でパケットの宛先を書き換えて認証サーバに転送します。もしホストが認証できると、認証サーバはコントローラに認証できたホストを通知します。コントローラはフローを更新し、認証が済んだホストのパケットを認証済ネットワークに転送します。こうして、認証が済んだホストのみが認証済みネットワークに接続できるようになります。

//image[secure_network][ホストを認識し、認証をするネットワーク][scale=0.5]

このように、コントローラは認証サーバのような外部システムと連携することでネットワークに機能を追加できます。コントローラは主要なプログラミング言語で実装できるので (@<chap>{openflow_frameworks})、さまざまな外部システムの API を通じて楽に連携できます。

=== 応用例 2: ロードバランサ

ルータに「流量を調べる」を追加して改造すると、いわゆるロードバランサを OpenFlow で実現できます (@<img>{load_balancer})。ロードバランサとは、Web サーバなどアクセスが集中しやすいサーバのスループットを上げるため、クライアントからのアクセスを複数台のバックエンドサーバに振り分けるネットワーク装置またはソフトウェアです。

//image[load_balancer][OpenFlow でロードバランサを実現する][scale=0.5]

ロードバランサは次のように動作します。

 1. クライアントからのリクエストが届くと、担当するバックエンドサーバを決める
 2. リクエストパケットをバックエンドサーバに届けるために書き換える (ルータと同じ)
 3. バックエンドサーバがつながるスイッチポートに書き換えたパケットを出力する

//noindent
アクセスの状況によっては、バックエンドサーバの間で負荷に偏りが出ることがあります。「流量を調べる」を使うと、空いているバックエンドへ優先して振り分けることができます。

必要なバックエンドサーバの数は、時間帯によって異なります。たとえば、アクセスが減る夜中になればバックエンドサーバ数を減らせます。逆に昼食後などのアクセスが増えやすい時間帯では、バックエンドサーバを増やす必要があります。

もしバックエンドサーバの数を調節できる API があれば、ロードバランシングとバックエンドサーバ数の調節を連携できます (@<img>{advanced_load_balancer})。「流量を調べる」でトラフィックを調べ、これに応じてバックエンドサーバ数を API 経由で自動調節できるからです。

//image[advanced_load_balancer][トラフィックに応じてバックエンドサーバの数を調節する高度なロードバランサの例。][scale=0.5]

=== 帯域をめいっぱい使う

「パケットを転送する」を使うと、複数の経路を使って VM イメージなどの大きなデータを効率的に転送できます。転送元から転送先へ複数のコネクションを張り、それぞれ専用の経路をめいっぱい使って転送できるのです。

//image[maximize_bandwidth][複数経路を使って帯域をかせぐ][scale=0.5]

この方法は複数の転送を同時に行う場合にも役立ちます。@<img>{maximize_bandwidth} と同じネットワーク構成で 3 つの異なるデータ転送を同時にやる状況を考えてください。もし 3 つすべてが上のリンクを使ってしまうと混んでしまいます。しかし@<img>{sharing_bandwidth} のように上下のリンクを共用すればより速く転送できます。

//image[sharing_bandwidth][3 つのアプリで帯域をシェア][scale=0.5]

=== ネットワークを冗長化する

「分岐する」で冗長な経路を作ってやれば、ネットワークに障害が起こった場合でもパケットをロスせずに復旧できます。クライアントとサーバが通信する状況を考えてください (@<img>{fail_over})。一番左のスイッチはパケットを複製し、2 つの経路に冗長化します。受け取る側は、冗長化された通信のうちどちらか一方だけを受け取るようにしておきます。このようにしておけば、もし下の経路に障害が起こっても、パケットを一つも落とさずに上の経路に切り替えて通信し続けられます。

//image[fail_over][OpenFlow で冗長な経路を作る][scale=0.5]

障害が起こった後もとの冗長構成に戻したい場合には、下のスイッチに「転送する」フローを新たに打ち込むことで迂回路を作れます (@<img>{advanced_fail_over})。

//image[advanced_fail_over][冗長な経路を作りなおす][scale=0.5]

ここで重要なのは、コントローラは全体の状況を見て自由自在に経路を決めることができるということです。従来のネットワークでは、各スイッチが個別に最適となるような経路を決めるため最短路など特定の経路しか取れませんでした。逆に OpenFlow では、コントローラが全体の道路と交通状況を逐一把握しており、好きなポイントで自由に経路を切り替えられます。

=== 応用例 3: 自由なネットワーク構成

OpenFlow を使うと、ネットワーク構成を物理的な構成にしばられることなく自由に構成できます。「パケットを転送する」を使うとホストの所属するネットワークを簡単に切り替えられます。@<img>{patch_panel} のようにスイッチにホスト 2 つとネットワーク 2 つが接続していると考えてください。コントローラはこれに「パケットを転送する」フローを打ち込むことで、ホストとネットワークの接続を自由に切り替えられます。たとえばホスト A がつながるポートとネットワーク B のつながるポートでパケットを相互に転送すれば、ホスト A はネットワーク B に属するようになります。

//image[patch_panel][各ホストの所属するネットワークを切り替える][scale=0.5]

こうした機能を持つハードウェアをパッチパネルと呼び、@<chap>{patch_panel}で詳しく説明します。より高度な例については、@<chap>{sliceable_switch}で紹介します。

== まとめ

OpenFlow がうまく行く具体的なユースケースを見てきました。OpenFlow はプログラミング次第で何にでもなれるオールマイティな存在ですが、機能面や信頼性では専用機にはかないません。データセンターのようなすべてをソフトウェア制御する用途で威力を発揮します。

次章では OpenFlow の仕様を少し詳しく紹介します。
