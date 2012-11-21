= OpenFlowって何？

//lead{
近ごろ話題のOpenFlowって、どんな仕組みで何がうれしいのでしょうか？難しいネットワーク用語を使わずに、身近なたとえ話で解説します。
//}

//indepimage[incredible_machine][][width=8cm]

== ソフトウェアで思いどおりにコントロールする

//quote{
無精(Laziness)：エネルギーの総支出を減らすために、多大な努力をするように、あなたをかりたてる性質。こうして労力を省くために書いたプログラムは他人も使うようになり、そのプログラムに関する質問にいちいち答えずに済ますためにドキュメントを書くようになる。それゆえ、プログラマにとって最も重要な素質である。またそれゆえ、この本が存在するのである。(『プログラミングPerl』、Larry Wallら、オーム社)
//}

優れたプログラマが持つハッカー気質の1つに「無精」があります。大好きなコンピュータの前から一時も離れずに、どうやってジャンクフードを手に入れるか…普通の人からするとただの横着に見えるかもしれませんが、ハッカー達にとってそれはいつでも大きな問題でした。

ソフトウェアによる横着はハッカーが最も創造性を発揮する分野の一つです。次の3つの話は、いずれもただ横着のためだけに高い技術力を駆使したというすごい例です。

 1. ハッカーの巣窟として有名なMITのAIラボにはかつて、コンピュータからオンラインでピザを注文できるUNIXコマンドが存在しました@<fn>{xpizza}。ハックしていて腹が減ったらコマンドをたたいてピザを取る、なんとも横着です。
 2. コンピュータサイエンスの名門、カーネギーメロン大学にはコーク・マシンという変わったコーラ自販機があり、UNIXコマンド一発でコーラの冷え具合を確認できるようになっています@<fn>{coke_machine}。遠くの自販機まで行って@<bou>{ぬるい}コーラをつかまされないための工夫です。
 3. RFC(Request For Comment)で標準化されているコーヒーポットプロトコルでは、遠隔地にあるコーヒーポットのコーヒーの量を監視したり、コーヒーを自動的にいれたりするための半分冗談のインターフェースを定義しています@<fn>{rfc2324}。本当に実装してしまった人もいたそうですから驚きです。

//footnote[xpizza][MITの@<tt>{xpizza}コマンドのマニュアル：@<href>{http://stuff.mit.edu/afs/sipb/project/lnf/other/CONTRIB/ai-info}]
//footnote[coke_machine][カーネギーメロン大のコーク・マシンのサイト：@<href>{http://www.cs.cmu.edu/~coke/}]
//footnote[rfc2324][RFC 2324：@<href>{http://www.ietf.org/rfc/rfc2324.txt}]

こうした「ソフトウェアで楽をする」ハックのうち、もっとも大規模な例が最新鋭のデータセンターです。クラウドサービスの裏で動くデータセンターは極めて少人数のエンジニアが運用しており、大部分の管理作業をソフトウェアによって極限まで自動化している、という記事を読んだことがある人も多いでしょう。このようにピザやコーラ、コーヒーのようなお遊びから、データセンターのように一筋縄ではいかない相手まで、ソフトウェアで「モノ」を思いどおりにコントロールするのは何よりも楽しく、そして実際に役に立つハックの一種です。

== SDN：ネットワークをソフトウェアで制御しよう

その中でもネットワークをハックする技術の1つが、本書で取り上げるOpenFlowです。簡単に言えば、OpenFlowはネットワークスイッチの動作を変更する標準プロトコルの1つです。OpenFlowを使えばスイッチ1つひとつの動作を自由に書き換えられるので、究極的にはネットワーク全体をソフトウェアで自由に制御できるようになります。これをSoftware Defined Networking(SDN、ソフトウェアで定義されるネットワーク)と呼び、OpenFlowはSDNを実現する代表的な技術として注目を受けています。

OpenFlowの登場によって、これからはネットワークもプログラミングの対象になります。「いまだにネットワークを手で管理してるの？そんなのソフトウェアで横着できるぜ？」こんな声が聞こえてきそうです。たしかに、今までネットワーク管理と言えば専門のオペレータ達による手作業がメインでした。横着できる部分はたくさんあるはずです。

ハッカーに負けない創造性とOpenFlowのプログラマブルな特性が組み合わされば、次のような「究極の自動化」も夢ではなくなります。

 * 障害やトラフィック情報など、あらゆる情報を収集し集中管理できるネットワーク。
 * ユーザやアプリケーションの追加・削除に応じて、自動的に構成を変更するネットワーク。
 * 追加投資をしなくても、既存のインフラをめいっぱい使えるように最適化するネットワーク。

本書はこのすべてのトピックを扱います。自宅や職場のような中小規模ネットワークからデータセンターのような超大規模ネットワークまで、具体的なOpenFlowの適用例を見ながら「OpenFlowってどんなもので、具体的に何に使えるのだろう？」という素朴な疑問に答えます。また「さっそくOpenFlowを使ってすごいネットワークを作ってみたい!」というプログラマ向けには、実際に動かせる実用的なコードをたくさん載せました。

本書を読み進めるにあたって、ネットワークやプログラミングの深い知識は不要です。基本から1つひとつ説明しますので、ネットワークの専門家はもちろん、プログラマやシステムエンジニア、そして営業職や管理職などなどOpenFlowに興味を持つ方であれば誰でもすんなり理解できるように構成してあります。ではさっそく、OpenFlowの仕組みを理解しましょう。

== OpenFlowの仕組み

OpenFlowの仕組みを説明するために、ここからちょっとした@<bou>{たとえ}話をします。みなさんもきっと利用したことがあると思いますが、よくあるカスタマーサポートを思い浮かべてください。そう、テレビとかパソコンの調子が悪くなったときに、フリーダイヤルで相談するアレです。それって、どこがOpenFlowと関係あるのでしょう？

実はOpenFlowの基本的な仕組みはカスタマーサポートにとても良く似ているのです。これからお話しする2つのストーリーが分かれば、OpenFlowの95%を理解できたも同然です。それではさっそく、このストーリーの主人公の@<ruby>{友太郎,ゆうたろう}君と、カスタマーサポートセンターで働く青井さん、そして上司の宮坂主任の3人に登場してもらいましょう。

=== ストーリー1：エアコンが壊れた

今年もエアコンの活躍する季節がやってきました。ところが友太郎君のエアコンはどうにも調子が良くありません。そこで取扱説明書に載っていたカスタマーサポートに電話することにしました。自動音声に従って問題ありそうな項目をすべてチェックしてみましたが、いっこうに解決しません。結局、自動音声はあきらめて電話オペレータに相談することになりました。

「はい、こちらカスタマーサポートセンターです。担当はわたくし青井がうけたまわります。ご用件は何でしょうか？」

青井さんはヨーヨーダイン・エアコン社で働く電話オペレータです。お客さんから不具合の症状を聞き出し、問題を解決できる適切なエンジニアに電話をつなぐのが仕事です(@<img>{yoyodyne_support})。

//image[yoyodyne_support][電話オペレータはお客さんからの問い合わせを適切なエンジニアへ転送][width=12cm]

「なんだかリモコンの調子が悪いんです。温度表示がずっと点滅しているんですけど、どうしたら直りますか?」

青井さんは手元の対応マニュアルを開きます(@<table>{manual1})。対応マニュアルには故障の箇所と、それを直せるエンジニアの内線番号が書かれています。続く数字はそれぞれの問い合わせ件数です。

//table[manual1][電話オペレータ用対応マニュアル]{
故障の箇所			担当エンジニアの内線番号	問い合わせ件数
------------------------------------------------------------------
リモコン			555-2222					8件
エアコン本体		555-4444					6件
室外機				555-3333					4件
//}

ちょうどマニュアルの先頭に、探していた項目がみつかりました。

「ご不便をおかけしました。担当のエンジニアにただいまおつなぎいたします」

電話の転送を終えると、青井さんはリモコン故障の問い合わせ件数を8件から9件にアップデートしました(@<table>{manual2})。

//table[manual2][問い合わせ件数をアップデートする]{
故障の箇所			担当エンジニアの内線番号	問い合わせ件数
------------------------------------------------------------------
リモコン			555-2222					@<em>{9件}
エアコン本体		555-4444					6件
室外機				555-3333					4件
//}

//noindent
こうすることでどんな故障の問い合わせが多いかを社内にフィードバックできます。たとえば開発部署は次の製品開発にこの情報を生かせるというわけです。

==== これをOpenFlowに置換えると…

OpenFlowの世界では、パケットを送信するホストがお客さんの友太郎君、パケットを転送するOpenFlowスイッチが電話オペレータの青井さんに対応します(@<img>{openflow_host_switch})。ホストがパケットを送ると、OpenFlowスイッチはパケットの中身に応じてパケットを適切な宛先に転送します。これはちょうど、青井さんが友太郎君からの問い合わせ内容に応じて適切な担当エンジニアに電話を転送するのと同じです。

//image[openflow_host_switch][OpenFlowではホストがお客さん、スイッチが電話オペレータ、そしてフローテーブルがマニュアルに対応][width=12cm]

OpenFlowスイッチは、その動作が「マニュアル化」されています。カスタマーサポートの例では、青井さんはマニュアルから転送先の番号を調べました。OpenFlowスイッチは、パケットの転送先をフローテーブルと呼ばれるデータベースを参照して決めます。青井さんの業務がすべてマニュアル化されているのと同じく、OpenFlowスイッチの動作はすべてこのフローテーブルの中身のみによって決まります。

==== 転送情報を管理するフローテーブル

フローテーブルには、「こういうパケットが届いたら、ポートx番に転送する」というルールがいくつか記録されています。このルールをフローエントリと呼びます。フローエントリはちょうど「リモコンの故障に関する問い合わせが来たら、内線555-2222に転送する」といったマニュアルの各項目に対応します。

実際のフローテーブルの例を少し見てみましょう。@<table>{story1_openflow}はあるスイッチのフローテーブルで、各行がひとつひとつのフローエントリに対応します。フローエントリはマッチングルール、アクション、そして統計情報の3つの要素から構成されます。

//table[story1_openflow][フローテーブルとフローエントリの例]{
マッチングルール						アクション			統計情報
-------------------------------------------------------------------------------
送信元IPアドレスが192.168.1.100			ポート8番に転送		80パケット
宛先IPアドレスが192.168.10.92			ポート10番に転送	14パケット
送信元MACアドレスが00:50:56:c0:00:08	ポート1番に転送		24パケット
//}

: マッチングルール
  マッチングルールは届いたパケットをどう処理するかをフローテーブルから探す「キー」として使われます。たとえば「リモコンの調子がおかしい」という問い合わせから転送先を決めたように、「パケットの送信元IPアドレスが192.168.1.100である」といったマッチングルールからパケットの転送先を決めます。

: アクション
  アクションは届いたパケットをどう扱うかという「対応内容」にあたります。たとえば「内線555-4444に転送」と同じく、アクションには「スイッチのポート8番に転送」などと指定します。なお、アクションでは単純な転送だけでなくパケットの書き換えもできます。

: 統計情報
  統計情報はフローエントリごとのパケット処理量の記録です。たとえば「リモコン関連の問い合わせ数は9件」とマニュアルに記録したように、「このフローエントリに従って転送したパケットは80個」などといった情報が書き込まれます。

いかがでしょうか？カスタマーサポートとOpenFlowは驚くほどよく似ていることがわかると思います。実はOpenFlowはとても単純で理解しやすい仕組みなのです。

=== ストーリー2：またエアコンが故障

エアコンもしばらくは順調でしたが、1か月後また調子が悪くなってしまいました。友太郎君はふたたびカスタマーサポートへダイヤルします。

「エアコンの排水ホースがすぐに詰まっちゃうんです」

青井さんはいつものように手元の対応マニュアルを調べましたが、困ったことに排水ホースの項目は載っていません。どうやらまったく新しい不具合のようです。

「すみませんが少々お待ちください。対応可能なエンジニアがいるかどうか確認いたします」

そして電話口には録音された "しばらくお待ちください" のメッセージとどこか軽快な音楽が流れはじめました。

//image[yoyodyne_support_miyasaka][対応マニュアルに対処法が見つからなかった場合、上司に聞く][width=12cm]

こういう時、青井さんがいつも頼るのは上司の宮坂主任です(@<img>{yoyodyne_support_miyasaka})。

「宮坂さん、排水ホースについての問い合わせが来ているのですが、誰につなげばよいですか?」

「それだったら山本君が適任だ」

転送先が分かった青井さんは、友太郎君の待つ電話に戻ります。

「大変お待たせいたしました。担当のエンジニアに転送いたします」

一度目の問い合わせと比べてかなり時間がかかってしまいましたが、これでようやく一件落着です。さらに青井さんは、宮坂主任から教わった山本君の内線番号をマニュアルに追加します(@<table>{manual3})。次からの同じ問い合わせにすばやく答えられるようにするためです。

//table[manual3][マニュアルに新しい症状と転送先を追加してアップデート]{
故障の箇所				担当エンジニアの内線番号		問い合わせ件数
----------------------------------------------------------------------
リモコン				555-2222						9件
エアコン本体			555-4444						6件
室外機					555-3333						4件
@<em>{排水ホース}		@<em>{555-5555}					@<em>{1件}
//}

==== これをOpenFlowに置換えると…

OpenFlowでこの上司にあたるのがコントローラと呼ばれるソフトウェアです(@<img>{openflow_host_switch_controller})。OpenFlowでネットワークをプログラミングする場合、プログラマが書くのはこのコントローラの部分です。頭脳であるコントローラをソフトウェアとして記述することで、ネットワークを自由自在に制御できるというわけです。

//image[openflow_host_switch_controller][フローテーブルにパケットのエントリが見つからなかった場合、コントローラに問い合わせる][width=12cm]

パケットはスイッチが高速に転送してくれますが、フローテーブルに載っておらずスイッチ側でどう処理してよいかわからないパケットが届くこともあります。この場合スイッチはこのパケットをコントローラに上げて「このパケットはどうすればよいですか？」と指示をあおぎます。コントローラはこのパケットの中身を調べ、どうすべきかという指示、つまりフローエントリをフローテーブルに書き込んでやります。

このようにフローテーブルに載っていないパケットが届くと、コントローラへの問い合わせが発生するのでパケット転送がとても遅くなります。しかし、スイッチの起動時にコントローラが必要なフローエントリをあらかじめ書き込んでおくようにしておけば、スイッチ側だけで素早く処理できます。

=====[column] @<ruby>{友太郎,ゆうたろう}の質問：コントローラへの問い合わせはどのくらい遅い？

フローテーブルを使わずに、毎回コントローラが指示を出すとどうなるでしょうか？結果は、何倍も遅くなります。試しにソフトウェアスイッチで転送する場合とコントローラですべて処理する場合を実験してみたところ、性能が5倍も遅くなってしまいました。もちろんこれはおおざっぱな値ですが、数倍は遅くなるという目安になります。また今回の実験はソフトウェアスイッチでしたが、ハードウェアスイッチを使うとこの差はさらに広がります。

=====[/column]

== OpenFlow のうれしさ

OpenFlowの仕組みの大枠は理解できたと思います。それでは最も肝心な部分、「OpenFlowって何がうれしいの？」を掘り下げてみましょう。

=== 自動化やシステム連携がしやすい

カスタマーサポートセンターでは、あらかじめ適切なマニュアルを作っておけば業務はすべて電話オペレータが自動的にやってくれます。これによって、全体の監督は管理職で実務は電話オペレータ、というふうにきっちりと分業できるようになります。たとえば電話オペレータが実務をやってくれている間、管理職は他の部署との協業や調整に集中できます。

同様に、OpenFlowスイッチの制御はすべてソフトウェアであるコントローラで実現しているのでネットワーク管理の自動化が容易です。さらにコントローラがRubyやPython、Javaなどのよく知られた汎用言語で書いてあれば、既存のシステムやサービスなどとの連携も簡単です。たとえば、アプリケーションからの要求やビジネスポリシーの変更、問題発生などさまざまなトリガーに応じてネットワークの設定を変更するといった、一歩進んだ自動化もできます。

=== ネットワークトラフィックを集中制御しやすい

カスタマーサポートセンターでは問い合わせ件数の情報はすべて管理職に上がってくるため、混み具合の把握や全体の交通整理が楽です。もし特定のエンジニアに問い合わせが集中しても、問い合わせがうまくばらけるようにマニュアルを通じて電話オペレータの全員に指示できます。反対にもし各オペレータが個々に判断してしまうと、おなじエンジニアに問い合わせが偏ることは避けられません。

OpenFlowでもすべてのトラフィック情報はコントローラに上がってくるため、全体を見たトラフィックの最適化が可能です。各種統計情報を集計することで、コントローラはネットワーク全体のトラフィックデータを集められます。そしてその情報をもとに各スイッチのフローテーブルを更新することで、全体的に見て最適となるパケット転送経路を設定できます。反対にもし個々のスイッチが判断してしまうと、うまくトラフィックを分散できません。

=== ソフトウェア開発のテクニックやツールが使える

コントローラはソフトウェアの一種なので、ソフトウェア開発で長年培われているさまざまなテクニックやツールをネットワーク構築に応用できます。

 * 近年主流のアジャイル開発手法でコントローラを開発すれば、反復的な機能追加が可能です。フィードバックを受けながらちょっとずつバージョンアップしていくことで、ネットワークを段階的に構築できます。
 * コントローラのユニットテストや受け入れテストを書くことで、ネットワーク全体を自動的にテストできます。テスト結果の出力は、そのまま仕様書の一部になります。ExcelやWordで書いた仕様書を別個に管理する必要はありません。
 * これはやや飛んだアイデアですが、コントローラのソースコードや関連データをgitなどのバージョン管理ツールで管理すれば、ネットワーク全体のバージョン管理やバージョン間の差分のチェック、および巻き戻しができるでしょう。

====[column] @<ruby>{取間,とれま}先生曰く：OpenFlowは回転ずし！？

従来のルータやスイッチは、ベンダが提供する機能をそのまま使うしかありませんでした。たとえば、100個ある機能のうち、本当に使いたい機能は10個だけだったとしても、100機能つきのルータを買うしかありません。これではある意味、フルコースしか頼めないフレンチレストランのようなものです。一部の機能しか利用していないのに障害ポイントが無数にあるので、切り分けやデバッグが難航することもままあります。

OpenFlowは回転ずしです。フランス料理の味に近づけるのは大変ですが、必要な機能だけをチョイスしてがんばって実装すれば、思い通りの機器が手に入るのです。

====[/column]

== OpenFlowで気をつけること

もちろん、OpenFlowでもうれしいことばかりではありません。コントローラで制御を一手に引き受けるというモデルになっているため、スイッチの台数が増えたときのスケーラビリティが問題になります。もし、フローテーブルに載っていないパケットが一気にコントローラへ到着すると、最悪の場合コントローラが停止してしまいます。

そこで、OpenFlowの使いどころやフローテーブルの残り容量には特に注意する必要があります。たとえばOpenFlowスイッチをインターネットのような多種多様のパケットが流れる環境につなげると、すぐにコントローラへの問い合わせが殺到しフローテーブルがいっぱいになって破綻してしまいます。しかしデータセンターなどの閉じた環境では、トラフィックの特徴や流れるパケットの種類はあらかじめ見当を付けておけます。そこで最低限のパケットのみがコントローラへ上がってくるようにうまくフローエントリとネットワークを設計することで、スイッチが増えてもスケールさせることが可能です。

== まとめ

本章ではSDNを実現するための部品であるOpenFlowを解説しました。OpenFlowはフローテーブルを持つスイッチと、フローテーブルの内容を集中管理するソフトウェアであるコントローラから成ります。ネットワーク管理をソフトウェア化することによって、自動化やさまざまなシステムとの連携、トラフィック制御のしやすさ、ソフトウェア技術の応用などなど種々の恩恵があります。

次章では、具体的なOpenFlowの使いどころをいくつか見ていきましょう。
