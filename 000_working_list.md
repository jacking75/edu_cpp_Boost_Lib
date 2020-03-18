# 정리 작업 리스트
https://www.boost.org/  
  
[(일어)boost 라이브러리 정보](http://boostjp.github.io/  )    
  
Boost.HOFについて  https://nekko1119.hatenablog.com/entry/2018/08/24/010924   
Boost String Algorithms Library http://d.hatena.ne.jp/kanetai/20111017/1318846372      
Boost で文字列の置換  http://d.hatena.ne.jp/osyo-manga/20120512/1336811661  
参照を保持するコンテナ  http://faithandbrave.hateblo.jp/entry/20110519/1305789940  
boost::in_place  http://nekko1119.hatenablog.com/entry/20121122/1353577143     
プラグインシステムを Boost.DLL で簡単に実装する紹介  http://qiita.com/usagi/items/0722550dc5433bd07139   
  
  
Boost offset_ptrと共有メモリとmmap  http://www.jinmei.org/blog/2012/05/20/3126   
PropertyTree  http://d.hatena.ne.jp/tt_clown/20091022/1256205859   
boost로 선현대수 입문  http://www.slideshare.net/wof_moriguchi/boost-8139529  
  
boost::numeric::interval<int>  
http://qiita.com/t_uda/items/e4f554b8af864b638a6d   
http://qiita.com/t_uda/items/abcf31a7b1787604a4f4   
http://qiita.com/items/e08ca64b318917e6d4ab   
boost::numeric::interval<int>(0): C++ Boost 区間演算ライブラリ 概要  http://qiita.com/t_uda/items/7712671389e016d24df6   
  
Boost.Flyweightのお勉?  http://tips.hecomi.com/entry/20111022/1319302933    
boost::multiprecision の使い方メモ  http://qiita.com/tubo28/items/fa8ee013390184b0ba18     
boost::processの子プロセスの入力を閉じる    https://qiita.com/cielavenir/items/9219162170cf2dd8b144  
BoostCon 2011の「Practical SIMD acceleration with Boost.SIMD」を日本語?しました  http://www.slideshare.net/faithandbrave/boostsimd     
  
  
  
##TypeErasure 
Boost.TypeErasure を使ってみる  http://d.hatena.ne.jp/osyo-manga/20121202/1354456126   
Boost.TypeErasureのドキュメントを翻訳してみた  http://d.hatena.ne.jp/gununu/20130705/1372983790  
  
  
  
## TTI
Boost.TTI を使ってみる  http://d.hatena.ne.jp/osyo-manga/20130707/1373170326   
Boost.TTI  http://d.hatena.ne.jp/osyo-manga/20111230/1325213907   
C++03で、特定のメンバ関数が存在するかを判定する http://faithandbrave.hateblo.jp/entry/2014/04/14/152959   
  
  
  
## 컨테이너
boost::container で俺俺アロケ?タ  http://www.6809.net/tenk/?%E9%9B%91%E8%A8%98%2f2012-12-14  
stable_vector  http://d.hatena.ne.jp/fjnl/20111207/1323269108  
static_vector  http://d.hatena.ne.jp/faith_and_brave/20130712/1373614497   
boost 1.48.0 flat_set はえー  http://blogs.wankuma.com/episteme/archive/2012/01/21/250181.aspx  
Boost.Heap コンテナの設定  http://d.hatena.ne.jp/faith_and_brave/20120510/1336641311  
Intrusive data structure 소개  http://www.slideshare.net/ohyecloudy/intrusive-data-structure   
Boost.Containerのフツーじゃないコンテナたち  http://codezine.jp/article/detail/8259   
static_vectorとsmall_vector https://qiita.com/shohirose/items/54d75b12abd452ee2786   
  
  
  
## multi_index
Boost 설명서 http://www24.atwiki.jp/reisiki/pages/59.html   
간단 사용법 http://d.hatena.ne.jp/osyo-manga/20101030/1288408414 , http://grayhole.blogspot.com/2009/03/boostmultiindex.html   
편리한 조건에 대해서 http://d.hatena.ne.jp/sakura-1/20090816/1250429894  ,  http://d.hatena.ne.jp/sakura-1/20090818#1250599374  
바운드리 검색 http://www.boost.org/doc/libs/1_47_0/libs/multi_index/doc/tutorial/basics.html  
Key modify http://d.hatena.ne.jp/faith_and_brave/20100625/1277450332  
Boost.MultiIndex - member_offset http://d.hatena.ne.jp/faith_and_brave/20110510/1305001676   
(1.59 이상)Boost.Multi-index ランク付きインデックス http://faithandbrave.hateblo.jp/entry/2015/07/22/144646   
  
  
## Pool
shared_ptr는 내부에 참조 카운트 용으로 new에서 메모리를 확보하므로 메모리 풀과 결합하는 방법이 까다롭고 boost::intrusive_ptr와 조합하는 방법이 편하다.  
메모리를 확보한 순서로 메모리 해제를 하면 object_pool 이 boost::pool 보다 느리다.  
  
C++/Boost boost::poolの大まかな使い方   http://blogs.yahoo.co.jp/nanashi_hippie/52188424.html   
boostのobject_poolをスマートポインタで利用する   http://d.hatena.ne.jp/ytakano/20100317/1268778286   
  
  
  
## PP 
Boost PP  http://www.slideshare.net/Suikaba/try-to-use-boostmpl   
明日から使える boost/preprocessor/facilities/  https://dechimal.hatenadiary.com/entry/20101214/1292354432   
BOOST_PP_REPEATの仕組み  https://qiita.com/hikarin522/items/5329647ec2a542653f68  
BOOST_PP_REPEAT  http://nekko1119.hatenablog.com/entry/2013/05/01/010105   
Boost.PP 闇魔術を触ってみた  http://www.slideshare.net/DtYaZsK/ss-32425592  
  
  
  
## QVM
행열 계산 라이브러리.  
  
Boost QVM(Quaternions, Vectors, Matrices)に触ってみた  http://qiita.com/yacohana/items/c249ade084c41e68da29   
Boost.QVM入門  https://qiita.com/agate-pris/items/3e0bb170e5c6356df108   
  
  
  
## program_options
boost::program_options   http://nekko1119.hatenablog.com/entry/20130414/1365921531   
コマンドライン引?をboost::program_optionsで格納   http://tips.hecomi.com/entry/20110210/1297353366   
  
  
  
## Beast 
beast_http_server  https://github.com/0xdead4ead/beast_http_server   
Boost.Beast で websocket の handshake に custom request header をつける方法  https://qiita.com/UedaTakeyuki/items/9b8fcc344690b8e7cded  
  
  
   
## Preprocessor
세미나 자료  http://d.hatena.ne.jp/DigitalGhost/20091213/1260732009  
소개  http://cpplover.blogspot.com/2008/07/boostpreprocessor-meta-programming.html  
일본어 홈페이지  http://boost.cppll.jp/HEAD/libs/preprocessor/doc/index.html  
boost.Preprocessor - enum과 문자열의 매핑   http://ricanet.com/new/view.php?id=blog/080607  
Boost::preprocessor 로 템플릿 코드 생성  http://rein.kr/blog/archives/486  
boost::tuple 의 직렬화 코드	http://reiot.com/2009/02/17/boost-preprocessor/  
  http://www.kmonos.net/alang/boost/classes/preprocessor.html  
http://d.hatena.ne.jp/DigitalGhost/searchdiary?word=*[Preprocessor]&  
  http://d.hatena.ne.jp/mb2sync/archive?word=*[Boost.Preprocessor]  
  http://slashdot.jp/~A7M/journal/481116  
  http://idlysphere.blog66.fc2.com/blog-entry-175.html  
  http://cpplover.blogspot.com/2008/07/blog-post_17.html  
  
  
  
## mpl 
Try to use boost.mpl  http://www.slideshare.net/Suikaba/try-to-use-boostmpl     
'11年代のMPL  https://kikairoya.hatenablog.com/entry/20111206/1323183937   
  
  
  
## wave 
http://www.ruche-home.net/program/boost/wave   
  
## Boost.Context
Boost.Context について調べた  http://d.hatena.ne.jp/melpon/20111213/1323704464    
  
Continuation with Boost.Context  http://www.slideshare.net/faithandbrave/continuation-with-boostcontext   
  
Boost.Context 非同期?理を逐次的にする  http://d.hatena.ne.jp/faith_and_brave/20120329/1333008572     
  
boost.contextを使ってみました  http://d1z.cocolog-nifty.com/blog/2012/02/boostcontext-ac.html   
  
Boost.Contextでファイバ?ライブラリを??してみた  http://yohhoy.hatenablog.jp/entry/2012/04/10/003735   
   
  
Coroutine , fiber 実行コンテキストの抽象化ライブラリ達  
http://www.flast.jp/article/boost-21-sapporo/index.html   
http://www.flast.jp/article/boost-21-sapporo/execution.html   
  
Boost.Coroutineのローカル変数の寿命   http://d.hatena.ne.jp/y_mazun/20130205/1360077844   
  
Boost.Coroutine でフィボナッチ  http://d.hatena.ne.jp/osyo-manga/20130209/1360412742   
  
Boost.Coroutine  http://d.hatena.ne.jp/joynote/20131118/1384779314   
  
C++ のコルーチン boost::coroutines を使ってみる  http://qiita.com/kobake@github/items/506b5ff128dc1372cd1d   
  
C++14 & boost::{coroutines|signals2} -> yieldable task system  http://qiita.com/usagi/items/6dad042e20ef1fe9f91c   
  
Visual C++ CoroutineとBoost Coroutine  http://sayurin.blogspot.kr/2016/07/visual-c-coroutineboost-coroutine_9.html   
  
  
  
## Algorithm 
Algorithm clamp  
http://d.hatena.ne.jp/faith_and_brave/20120704/1341382789  
http://d.hatena.ne.jp/faith_and_brave/20120705/1341471158  
  
Algorithm hex  http://d.hatena.ne.jp/faith_and_brave/20120706/1341558540  
  
Boost.Algorithm ?索アルゴリズム  http://d.hatena.ne.jp/faith_and_brave/20120709/1341820912  
  
  
  
## Boost::msm
Boost.Msm 사용 방법  http://www.slideshare.net/redboltz/boost-sapporomsm-pre   
  
https://sites.google.com/site/boostjp/tips/finite_state_machine   
http://d.hatena.ne.jp/osyo-manga/20100917/1284716191   
http://d.hatena.ne.jp/kura-replace/20110514/1305385110   
http://d.hatena.ne.jp/redboltz/20110625/1308997694  
http://d.hatena.ne.jp/redboltz/20110626/1309046959  
http://d.hatena.ne.jp/redboltz/20110626/1309074006  
  
if/else分岐  http://d.hatena.ne.jp/redboltz/20110722/1311312541   
  
Boost.Msmがサポ?トしないUML表記とその??法  http://d.hatena.ne.jp/redboltz/20110722/1311297996   
  
http://d.hatena.ne.jp/redboltz/20110726/1311682730
  
開始疑似?態と?行?態  http://d.hatena.ne.jp/redboltz/20110803/1312336449   
  
entry point疑似?態の??  http://d.hatena.ne.jp/redboltz/20110803/1312359024  
  
状態マシンの内部で別のイベントを発生させる http://d.hatena.ne.jp/redboltz/20110813/1313244419  
  
イベントの延期(defer)  http://d.hatena.ne.jp/redboltz/20110815/1313388757  
  
遷移テ?ブルの評?順序を上から下にする  http://d.hatena.ne.jp/redboltz/20110818/1313655344  
  
entry pseudo stateのリージョンIDの省略  http://d.hatena.ne.jp/redboltz/20120306/1331030665  
  
entry point疑似?態の??の制約とその??策  http://d.hatena.ne.jp/redboltz/20120603/1338706853  
  
  
  
## Hana
Boost.Hana の紹介！  https://qiita.com/alphya/items/94dcfc4dd08445f9c62b  
Use the official Boost.Hana with MSVC 2017 Update 8 compiler  https://blogs.msdn.microsoft.com/vcblog/2018/08/30/use-the-official-boost-hana-with-msvc-2017-update-8-compiler/   
  
  
  
## Xpressive
Boost.Xpressive で HTML タグの中身とタグ名を取得する  http://d.hatena.ne.jp/osyo-manga/20120516/1337152762
  
일본어 문서  http://alpha.sourceforge.jp/devel/boost.xpressive_ja_1_45_0.pdf  
  
  
  
## Spirit
한글 번역 문서  http://jacking75.cafe24.com/Boost/Spirit/index.htm  
최신 버전 문서  http://www.boost.org/doc/libs/1_42_0/libs/spirit/doc/html/index.html  
버전 2에 대한 간단한 설명  http://www-ise2.ist.osaka-u.ac.jp/~kimura/boost/?Spirit  
  
< 예제 >  
http://www.kmonos.net/alang/boost/classes/spirit.html  
         http://www.fides.dti.ne.jp/~oka-t/cpplab-boost-spirit.html  
          http://tamachan.club.kyutech.ac.jp/~gridbug/spirit/  
    	http://www.kanetaka.net/4dapi/wiki4d.dll/4dcgi/wiki.cgi?Boost::Spirit=  
  	http://d.hatena.ne.jp/amachang/20081002/1222977482  
JSON 파서  http://babei.sblo.jp/article/24971565.html  
   
  
 
# Proto
boost 라이브러리의 Spirit, Phoenix, MSM, Xpressive가 Proto 사용  
  
시작  http://d.hatena.ne.jp/faith_and_brave/20090501/1241167261  
Boost.Proto  http://www.slideshare.net/fjnl/boost4-boostproto  
http://0x35.tumblr.com/post/16706887475/boost-proto  
  
   
  
## 직렬화
Boost  직렬화에서 사용하고 있는 싱글톤   http://d.hatena.ne.jp/redboltz/20100226/1267140671  
  
  
  
## Geometry 
Boost.Geometryの設計がすごい  http://mmiyano.blogspot.com/2012/02/boostgeometry.html  
Boost.Geometryを使って2Dゲ?ム?たり判定で?をしたい  http://d.hatena.ne.jp/joynote/20111009/1318171529  
Boost.Geometry touches  http://d.hatena.ne.jp/faith_and_brave/20120718/1342597171   
  
  
  
## Boost.Interprocess
Boost.interprocess  http://sssslide.com/www.slideshare.net/krustf/boostinterprocess-10461625  
Boost.Interprocess 共有メモリ操作  http://nox-insomniae.ddo.jp/insomnia/2012/03/boost-interprocess-shm-operation.html   
Shared Memory Word Binary Tree  http://nox-insomniae.ddo.jp/insomnia/2012/04/shared-memory-word-binary-tree.html  
boost - Interprocess Communication, Shared Memory  http://jeremyko.blogspot.kr/2012/04/boost-interprocess-communication-shared.html 
http://jeremyko.blogspot.kr/2012/04/boost-interprocess-communication-shared_04.html  http://jeremyko.blogspot.kr/2012/04/boost-interprocess-communication-shared_2818.html  
boost::interprocess::message_queueを使ってプロセス間通信  https://qiita.com/ondorela/items/ba5ddc1cb0bef8d4b200   
  
  
  
## Fusion
Boost.Fusionでreplicate  http://d.hatena.ne.jp/faith_and_brave/20120720/1342768623   
Type Driven Wire Protocols with Boost Fusion  http://rodgert.github.io/2014/09/09/type-driven-wire-protocols-with-boost-fusion-pt1/   
  
  
  
## boost::signals2
튜토리얼  http://docs.google.com/Doc?id=ddcwmgjq_12hdhr8tcw  
boostjp  https://sites.google.com/site/boostjp/tips/signals   
boost::signals2 が便利そうな件について  http://d.hatena.ne.jp/gintenlabo/20091201/1259671954   
Observerパタ?ン  http://w.livedoor.jp/programming/d/Signals2   
Boost.Signals2 切?  http://d.hatena.ne.jp/faith_and_brave/20110131/1296458381   
Boost.Signals2 を Boost.Parameter を使用して定義する  http://d.hatena.ne.jp/osyo-manga/20110816/1313449833   
Boost.Signals2 で呼ばれる??に優先順位を付ける  http://d.hatena.ne.jp/osyo-manga/20110815/1313366926   
デリゲ?トまたはイベントハンドラをC++で??するならBoost.FunctionよりもBoost.Signals2を使おう。 http://d.hatena.ne.jp/poyonon/20111110/1320933772   
C++ で、Model-View-Controller とか  http://d.hatena.ne.jp/osyo-manga/20110721/1311240737   
シグナル・スロット(Signal/Slot)の仕組みをBoost.Signal2で試してみる  http://qiita.com/kaoru/items/b8b39899ee4c771c2518   
boost::signals2  http://nekko1119.hatenablog.com/entry/20121108/1352356571   
  
  
  
## Graph
boost::graphのお勉強  http://qiita.com/kktk-KO/items/6c53e1c550cae441c969   
boostのグラフ別，動作の違い  http://folioscope.hatenablog.jp/entry/2012/10/31/224238   
Boost.GraphでJR全線乗り尽くしプランを立てる - プログラミング生放送  http://www.slideshare.net/maraigue/chinese-postman
  
http://boost.cppll.jp/HEAD/libs/graph/doc/graph_theory_review.html 
http://d.hatena.ne.jp/faith_and_brave/20100413/1271129607
http://d.hatena.ne.jp/faith_and_brave/20100415/1271315024
http://d.hatena.ne.jp/faith_and_brave/20100416/1271388752
Boost.Graph 到達可能かどうかをチェックする
http://d.hatena.ne.jp/faith_and_brave/20111011/1318317668
Boost.Graph 最短?路の長さ(weight)を計算する
http://d.hatena.ne.jp/faith_and_brave/20111012/1318407981 
Boost.Graph filtered_graph
http://d.hatena.ne.jp/faith_and_brave/20111014/1318572553 
Boost.Graph is_reachableAdd Star
http://d.hatena.ne.jp/faith_and_brave/20111017/1318837488
Boost.Graph is_adjacent
http://d.hatena.ne.jp/faith_and_brave/20111018/1318915464 
Boost.Graph vectorを有向グラフとして使用する
http://d.hatena.ne.jp/faith_and_brave/20111019/1319002566
Boost.Graph 頂点と辺の数を取得
http://d.hatena.ne.jp/faith_and_brave/20111031/1320039384
 Boost.Graph PredecessorMap
 http://d.hatena.ne.jp/faith_and_brave/20111101/1320125923 
 Boost.Graph DistanceMap
 http://d.hatena.ne.jp/faith_and_brave/20111102/1320211726
Boost.Graph 最短経路を計算する過程を取得
http://d.hatena.ne.jp/faith_and_brave/20111115/1321338504
Boost.Graph 2つのグラフが同型かを調べる
http://d.hatena.ne.jp/faith_and_brave/20120612/1339489283 
Boost.Graph トポロジカルソート
http://d.hatena.ne.jp/faith_and_brave/20120613/1339577486 
グラフ理論 Welsh-Powellの頂点彩色アルゴリズムを実装してみた
http://d.hatena.ne.jp/devm33/20120616/1339811432
Boost.Graph Bundleプロパティ
http://d.hatena.ne.jp/faith_and_brave/20120619/1340093038
Boost.Graph 最短?純路
http://d.hatena.ne.jp/faith_and_brave/20120626/1340689362 
Boost.Graph 最小全域木を作る
http://d.hatena.ne.jp/faith_and_brave/20120627/1340783683 
Boost.Graph 一筆書き
http://d.hatena.ne.jp/faith_and_brave/20120628/1340866757
Boost.Graph 複?ソ?スの幅優先探索
http://d.hatena.ne.jp/faith_and_brave/20121120/1353405062 
Boost.Graph スモールワールドを作る
http://d.hatena.ne.jp/faith_and_brave/20121207/1354866489 
Boost.Graph 全頂点間最短距離を求め
http://d.hatena.ne.jp/faith_and_brave/20121212/1355295002 

astar_shortest_path.cpp
https://gist.github.com/faithandbrave/7331277 
 
Boost.Graphでの最短経路探索
http://goyoki.hatenablog.com/entry/2014/03/02/023758 

Boost.Graph Graphvizの情報をユーザー定義型に読み込む
http://faithandbrave.hateblo.jp/entry/2015/04/13/231836 

最短経路探索 (Boost Graph Library 利用でダイクストラ法)
https://qiita.com/kkdd/items/73fdc783799a71f19d15   
  
   
   
## Boost.Log
공식 http://www.boost.org/doc/libs/1_55_0/libs/log/doc/html/index.html   
  
Boost.Log使い始め
http://faithandbrave.hateblo.jp/entry/20130725/1374736211 

boost.Logの使い方を勉強した 
http://d.hatena.ne.jp/yamada28go/20140215/1392470561 

Boost.Log: コンソール画面とファイルにロギング
http://daisukekobayashi.com/blog/boost-log-logging-console-windows-and-file 

Boost.Log ファイルサイズによるローテーション
http://faithandbrave.hateblo.jp/entry/20130801/1375340888 

Boost.Logでファイルに出力する
http://kkayataka.hatenablog.com/entry/2013/07/29/062652
 
샘플
http://bitter-development.blogspot.kr/2013/08/boostlog-20.html

Boost.log Advaced 사용법
http://miragecore.tistory.com/entry/Boostlog-Addvaced-%EC%82%AC%EC%9A%A9%EB%B2%95
http://boost-log.sourceforge.net/libs/log/doc/html/index.html

この文書の読み方@Boost.Log 2.0
http://satoshinote.blogspot.kr/2013/06/boostlog-20_15.html

インストールと互換性@Boost.Log 2.0
http://satoshinote.blogspot.kr/2013/06/boostlog-20_16.html

定義@Boost.Log 2.0
http://satoshinote.blogspot.kr/2013/06/boostlog-20_17.html

動機＠Boost.Log 2.0
http://satoshinote.blogspot.kr/2013/06/boostlog-20.html

構成とライブラリーのビルド@Boost.Log 2.0
http://satoshinote.blogspot.kr/2013/06/boostlog-20_3584.html

non-trivialなBoost.Logの使い方
http://qiita.com/termoshtt/items/071fcefef4720dcd790d 
http://qiita.com/termoshtt/items/ee9a6df5d4a5f30a86f6 
http://qiita.com/termoshtt/items/6b25084556f8a69239cb 
http://qiita.com/termoshtt/items/9c856562fe8a8c42560c 

Boost.Logとfluentdで始めるログ活用術
http://www.slideshare.net/termoshtt/boostlogfluentd    