= OpenFlow のユースケース

//lead{
ざっくりと OpenFlow を理解したところで、さっそく実例です。OpenFlow が使えるユースケースとはどんなものでしょうか。
//}

OpenFlow スイッチには「コントローラさえ書けばどんなネットワーク機器にも化けられる」という特長があります。乱暴に言うと、スイッチはもちろんルータやロードバランサ、ファイアウォールや NAT など複雑な機器も、コントローラの実装を頑張れば実現できてしまいます。もちろん、専用機と同じ機能をすべて実装するのは不可能ですし、機能の一部をソフトウェアとして実装することになるので性能は落ちます。しかし、ソフトウェア次第で何でもできる事に変わりはありません。

この何でもできるという特長は、元々は大学や研究所などアカデミアからのニーズによって生まれたものでした。今までのスイッチやルータにとらわれない、まったく新しいインターネットを研究したい。でもすでにあるスイッチやルータのファームウェアを改造するのは大変、かと言って一からハードウェアは作りたくないし…大規模な仮想ネットワーク上で実験してもいいけど、それだと実際のインターネット環境とあまりにも違いすぎる。このジレンマを解消するために考え出されたのが OpenFlow だったわけです。

== なぜ OpenFlow が流行るのか？

では、インターネットの研究用だった OpenFlow がデータセンターに代表される産業界でも注目されるようになったのはなぜでしょうか？インターネットとデータセンターに求められる要件を比べてみましょう。

: インターネットの要件
  インターネットで重視されるのは相互接続性と信頼性です。インターネットには新旧さまざまなネットワーク機器がつながるので、たとえレガシーなプロトコルであろうとサポートしなくてはいけません。また、ルータなどのメンテナンスは人手で行われるので、ひんぱんに落ちるのも困ります。そのため、インターネットを構成するネットワーク機器には高い信頼性が求められます。

: データセンターの要件
  データセンターで重視されるのはスループットとコストです。Google などの巨大データセンターでは、全体のスループットをかせぐためにたくさんのスイッチやサーバをそろえる必要があります。ハイエンドのハードウェアは高くつくため、街の電気屋でも買えるごく普通のパーツから構成されるハードウェアを使います。個々の信頼性は落ちますが、そこはソフトウェアのレイヤで補います。管理は人手ではなくソフトウェアで行うため、ネットワークを含め全体をソフトウェア制御できなくてはいけません。

明らかに OpenFlow はインターネットの要件に合っていません。それにもかかわらず OpenFlow がインターネット研究に使われた理由は、実験ができればそれで良かったからです。自分の考えた方式をてっとり早く実装して実験できることが一番で、本格的な運用はその方式を元に製品化されたネットワーク機器を使えば良いのです。

一方で OpenFlow はデータセンターにうまくマッチしています。信頼性も機能も専用機にはかないませんが、コストが低くソフトウェアから制御できるという柔軟性があります。安い部品をたくさん集めて信頼性はソフトウェアで補う、というデータセンターのモデルに合致しています。

このように「柔軟性があればそこそこの信頼性や機能でよい」という OpenFlow の設計が、たまたまデータセンターにマッチした、これが OpenFlow が注目される理由です。

== ユースケースあれこれ

データセンターの中で使われる主なネットワーク部品には、スイッチ、ルータ、ロードバランサ、そして経路制御があります。今まで見てきたように、データセンターはとにかくスループット重視で多機能は重視せず、信頼性はその上のソフトウェアレイヤで担保します。このため、OpenFlow で実装する場合には最低限の機能さえあれば良いことになります。以降では、これらの部品を OpenFlow で実装する方法を説明します。その前に、OpenFlow でできることをおさらいしておきましょう。

「OpenFlow でできることは何か？」という質問を言いかえると、「フローでできることは何か？」になります。これもすでに@<chap>{whats_openflow}で触れましたが、フローでできることは基本的には次の 5 つです。

 1. パケットを転送する
 2. 破棄する
 3. 流量を調べる
 4. 書き換える
 5. 分岐する

昔懐かしい水道管ゲームを思い出してください。水道管のバルブから蛇口まで、さまざまな形の水道管のカードをならべて水を届けるゲームです。フローでできることはこれによく似ています。パケットを指定したスイッチポートから出力する。パケットのヘッダを書き換える。パケットを捨てる。パケットを複製して複数のスイッチポートから出力する。トラフィック量を調べる。これがすべてです。これを自由に組み合わせて、さまざまなタイプのネットワークを作れます。

それではいよいよ実際のユースケースを見ていきましょう。上の 5 種類のカードをどのように組み合わせて実現しているかというところに注目してください。

=== スイッチ

=== ルータ

=== ロードバランサ

「パケットを転送する」と「書き換える」、「流量を調べる」を組み合わせると、いわゆるロードバランサを OpenFlow で実現できます (@<img>{load_balancer})。ロードバランサとは、Web サーバなどアクセスが集中しやすいサーバのスループットを上げるため、クライアントからのアクセスを複数台のバックエンドサーバに振り分けるネットワーク装置またはソフトウェアです。

//image[load_balancer][OpenFlow でロードバランサを実現する][scale=0.5]

OpenFlow でロードバランサを実現する仕組みは次の通りです。

 * クライアントからのリクエストが届くと、担当するバックエンドサーバを決める
 * リクエストであるパケットの宛先をバックエンドサーバに書き換える
 * バックエンドサーバがつながるスイッチポートに書き換えたパケットを出力する。

//noindent
アクセスの状況によっては、バックエンドサーバの間で負荷に偏りが出ることがあります。「流量を調べる」を使うと、空いているバックエンドへ優先して振り分けることができます。

必要なバックエンドサーバの数は、時間帯によって異なります。たとえば、アクセスが減る夜中になればバックエンドサーバ数を減らせます。逆に昼食後などのアクセスが増えやすい時間帯では、バックエンドサーバを増やす必要があります。

もしバックエンドサーバの数を API から調節できる仕組みがあれば、ロードバランシングとバックエンドサーバ数の調節を連携できます (@<img>{advanced_load_balancer})。「流量を調べる」でトラフィックを調べ、これに応じてバックエンドサーバ数を API 経由で自動調節できるからです。

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

=== VM を移動する

OpenFlow ではパケットの中身を見て転送先を自由に決められるので、従来の IP アドレスによらない自由な転送方式を実装できます。これを使えば、たとえばデータセンター内での VM 移動など IP アドレスが変わると困るものを、IP アドレスを変えずに自由に転送できます。

//image[vm_migration][IP アドレスを変えずに VM を移動][scale=0.5]

=== セキュアなネットワークを作る

//image[secure_network][端末を認識し、認証と検疫をするネットワーク][scale=0.5]

== まとめ

OpenFlow がうまく行く具体的なユースケースを見てきました。OpenFlow はプログラミング次第で何にでもなれるオールマイティな存在ですが、とくに既存システムとの連携、全体最適化が得意分野です。新しいユースケース

次章では OpenFlow の仕様を少し詳しく紹介します。