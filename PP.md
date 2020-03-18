# Boost.PP
- boost/preprocessor.hpp 에 들어 있는 매크로 그룹

## BOOST_PP_IF(cond, t, f)
이것은 매크로 전개를 제어하는 것으로 아래처럼 사용한다.  
```
/* 전개 전 */
printf(BOOST_PP_IF(10, "TRUE", "FALSE"));
printf(BOOST_PP_IF(0, "TRUE", "FALSE"));

/* 전개 후 */
printf("TRUE");
printf("FALSE");
```
  
BOOST_PP_IF는 control/if.hpp 에서 아래처럼 정의 되어 있다.   
```
#define BOOST_PP_IF(cond, t, f) BOOST_PP_IIF(BOOST_PP_BOOL(cond), t, f)
```
  
여기에서 BOOST_PP_IIF 와 BOOST_PP_BOOL 이 나왔다.  
  
  
## BOOST_PP_BOOL(x)
이것은 인수를 판정해서 1 또는 0 으로 전개하는 매크로로 logical/bool.hpp 에서 아래처럼 정의 되어 있다.  
```
#define BOOST_PP_BOOL(x)    BOOST_PP_BOOL_I(x)
#define BOOST_PP_BOOL_I(x)  BOOST_PP_BOOL_ ## x
#define BOOST_PP_BOOL_0     0
#define BOOST_PP_BOOL_1     1
#define BOOST_PP_BOOL_2     1
...
#define BOOST_PP_BOOL_256 1
```
  
  
## BOOST_PP_IIF(bit, t, f)
이것은 bit가 0 이라면 f로, 1 이라면 t로 전개되는 매크로로 BOOST_PP_IF의 본체이다.  
  
control/iif.hpp 에 정의 되어 있다  
```
#define BOOST_PP_IIF(bit, t, f)    BOOST_PP_IIF_I(bit, t, f)
#define BOOST_PP_IIF_I(bit, t, f)  BOOST_PP_IIF_ ## bit(t, f)
#define BOOST_PP_IIF_0(t, f)       f
#define BOOST_PP_IIF_1(t, f)       t
```
  
전개는 아래처럼 된다  
```
BOOST_PP_IIF(1, "TRUE", "FALSE")
-> BOOST_PP_IIF_I(1, "TRUE", "FALSE")
-> BOOST_PP_IIF_ ## 1("TRUE", "FALSE")
-> BOOST_PP_IIF_1("TRUE", "FALSE")
-> "TRUE"
```
  
이것으로 BOOST_PP_IF 가 아래처럼 전개되는 것을 알 수 있다.  
```
BOOST_PP_IF(10, "TRUE", "FALSE")
-> BOOST_PP_IIF(BOOST_PP_BOOL(10), "TRUE", "FALSE")
-> BOOST_PP_IIF(1, "TRUE", "FALSE")
-> "TRUE"
```
  
  
토큰의 결합은 BOOST_PP_CAT을 사용하기도 한다.   
cat.hpp  
```
#define BOOST_PP_CAT(a, b)    BOOST_PP_CAT_I(a, b)
#define BOOST_PP_CAT_I(a, b)  a ## b
```
  
  
  
## BOOST_PP_REPEAT(count, macro, data)
さて本題 BOOST_PP_REPEAT です。
これはプリプロセッサで繰り返し処理をするためのマクロで以下のように展開されます。

/* 展開前 */
BOOST_PP_REPEAT(count, macro, data)

/* 展開後 */
macro(z, 0, data) macro(z, 1, data) ... macro(z, count - 1, data) 
なのでこれを使うと以下のようなことができます。(zは内部(ループの最適化)で使う値なので今は気にしなくで大丈夫です)

#define SUM(z, n, data)  BOOST_PP_EXPR_IF(n, +) BOOST_PP_INC(n)
// BOOST_PP_EXPR_IF(n, +)はn != 0 なら + に展開される
// BOOST_PP_INC(n)は n + 1 に展開される

BOOST_PP_REPEAT(10, SUM, _);
-> SUM(z, 0, _) SUM(z, 1, _) ... SUM(z, 9, _);
-> 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10;
これほんとにマクロなのか?
しかしBOOST_PP_REPEATの恐ろしさはこんなものではありません。なんと2重ループができるのです。

#define MUL(z, n, _)  BOOST_PP_EXPR_IF(n, *) (BOOST_PP_REPEAT(BOOST_PP_INC(n), SUM, _))
#define SUM(z, n, _)  BOOST_PP_EXPR_IF(n, +) BOOST_PP_INC(n)

BOOST_PP_REPEAT(10, MUL, _);
-> MUL(z, 0, _) MUL(z, 1, _) ... MUL(z, 9, _);
-> (BOOST_PP_REPEAT(1, SUM, _)) * (BOOST_PP_REPEAT(2, SUM, _)) * ... * (BOOST_PP_REPEAT(10, SUM, _));
-> (1) * (1 + 2) * (1 + 2 + 3) * ... * (1 + 2 + ... + 10);
あれ？おかしいですね。BOOST_PP_REPEATの展開中にまたBOOST_PP_REPEATが出てきています。プリプロセッサでは再帰できないはずなのに。

実際以下のようなマクロは正しく展開されません。

#define X(seq)            X_I seq
#define X_I(elem, isEnd)  X_ ## isEnd(elem)
#define X_0(elem)         elem + X_I
#define X_1(elem)         elem

X((a, 0)(b, 0)(c, 1))
-> a + X_I(b, 0)(c, 1)
これは

X((a, 0)(b, 0)(c, 1))
-> X_I(a, 0)(b, 0)(c, 1)
-> X_##0(a)(b, 0)(c, 1)
-> X_0(a)(b, 0)(c, 1)
-> a + X_I(b, 0)(c, 1)
-> a + b + X_I(c, 1)
-> a + b + c
と展開してほしいところですがX_Iの展開中にX_Iが再度出てきているため
a + X_I(b, 0)(c, 1)
までしか展開されません。※展開できるVerを記事の最後に書きました。

gccのドキュメントにもちゃんと書いてます。3.10.5 Self-Referential Macros

なぜBOOST_PP_REPEATは再帰できているのか。

repetition/repeat.hpp

#define BOOST_PP_REPEAT  BOOST_PP_CAT(BOOST_PP_REPEAT_, BOOST_PP_AUTO_REC(BOOST_PP_REPEAT_P, 4))
なるほど。関数マクロではないようです。
BOOST_PP_CATはトークンを結合するものなので BOOST_PP_REPEAT_ と BOOST_PP_AUTO_REC(BOOST_PP_REPEAT_P, 4) を結合しているのは分かりました。
でも BOOST_PP_AUTO_REC(BOOST_PP_REPEAT_P, 4) っていったい何者なんだ

BOOST_PP_AUTO_REC(pred, n)
これはあるマクロが展開済みかどうかを2分探索で判別し、その展開回数を返すマクロです。
ほんと何言ってるか訳が分からないと思いますが順番に見ていきましょう。
※ BOOST_PP_REPEATでは n == 4 なので4のやつだけ抜粋

detail/auto_rec.hpp

#define BOOST_PP_AUTO_REC(pred, n)  BOOST_PP_NODE_ENTRY_ ## n(pred)

#define BOOST_PP_NODE_ENTRY_4(p)    BOOST_PP_NODE_2(p)(p)

#define BOOST_PP_NODE_2(p)          BOOST_PP_IIF(p(2), BOOST_PP_NODE_1, BOOST_PP_NODE_3)
#define BOOST_PP_NODE_1(p)          BOOST_PP_IIF(p(1), 1, 2)
#define BOOST_PP_NODE_3(p)          BOOST_PP_IIF(p(3), 3, 4)
これは以下のように展開されます

BOOST_PP_AUTO_REC(BOOST_PP_REPEAT_P, 4)
-> BOOST_PP_NODE_ENTRY_4(BOOST_PP_REPEAT_P)
-> BOOST_PP_NODE_2(BOOST_PP_REPEAT_P)(BOOST_PP_REPEAT_P)
-> BOOST_PP_IIF(BOOST_PP_REPEAT_P(2), BOOST_PP_NODE_1, BOOST_PP_NODE_3)(BOOST_PP_REPEAT_P)
ここで BOOST_PP_IIF が出てきたので、これは BOOST_PP_NODE_1 又は BOOST_PP_NODE_3 に展開されます。

BOOST_PP_NODE_1 のとき (BOOST_PP_REPEAT_P(2) == 1)
-> BOOST_PP_IIF(BOOST_PP_REPEAT_P(2), BOOST_PP_NODE_1, BOOST_PP_NODE_3)(BOOST_PP_REPEAT_P)
-> BOOST_PP_NODE_1(BOOST_PP_REPEAT_P)
-> BOOST_PP_IIF(BOOST_PP_REPEAT_P(1), 1, 2)
また BOOST_PP_IIF が出てきたので 1 又は 2 に展開されます。

BOOST_PP_NODE_3 のとき (BOOST_PP_REPEAT_P(2) == 0)
これも同様に展開すると最終的に 3 又は 4 に展開されます。

表にすると以下のようになります。

展開後
BOOST_PP_REPEAT_P(2) == 1	BOOST_PP_REPEAT_P(1) == 1	1
BOOST_PP_REPEAT_P(2) == 1	BOOST_PP_REPEAT_P(1) == 0	2
BOOST_PP_REPEAT_P(2) == 0	BOOST_PP_REPEAT_P(3) == 1	3
BOOST_PP_REPEAT_P(2) == 0	BOOST_PP_REPEAT_P(3) == 0	4
こういうやつどっかで見たことありませんか?
そう、これは BOOST_PP_REPEAT_P(n) が1を返す最小のnを2分探索で求めていたやつなんですね。
※ BOOST_PP_REPEAT_P(n) == 1 なら BOOST_PP_REPEAT_P(n + 1) == 1 も成り立つ

これで BOOST_PP_AUTO_REC(BOOST_PP_REPEAT_P, 4) が 1 ～ 4 のどれかに展開されることが判明したので、BOOST_PP_REPEAT は以下のように展開されます。

BOOST_PP_REPEAT
-> BOOST_PP_CAT(BOOST_PP_REPEAT_, BOOST_PP_AUTO_REC(BOOST_PP_REPEAT_P, 4))
-> BOOST_PP_REPEAT_ ## 1 // n == 1
-> BOOST_PP_REPEAT_1
なので先ほどの2重ループの例は次のようになります。

#define MUL(z, n, _)  BOOST_PP_EXPR_IF(n, *) (BOOST_PP_REPEAT(BOOST_PP_INC(n), SUM, _))
#define SUM(z, n, _)  BOOST_PP_EXPR_IF(n, +) BOOST_PP_INC(n)

BOOST_PP_REPEAT(10, MUL, _);
-> BOOST_PP_REPEAT_1(10, MUL, _);
-> MUL(z, 0, _) MUL(z, 1, _) ... MUL(z, 9, _);
-> (BOOST_PP_REPEAT(1, SUM, _)) * (BOOST_PP_REPEAT(2, SUM, _)) * ... * (BOOST_PP_REPEAT(10, SUM, _));
なるほど。ここで2回目に出てきたBOOST_PP_REPEATがBOOST_PP_REPEAT_2になってくれればBOOST_PP_REPEAT_1の再帰にならずに済みそうです。
※ BOOST_PP_REPEATの展開はBOOST_PP_REPEAT_1に展開された時点でいったん終わっているので再帰にはなっていません。

やっとここまで来ましたがまだ BOOST_PP_REPEAT_P(n) が残っています。もうしんどい :innocent:

BOOST_PP_REPEAT_P(n)
repetition/repeat.hpp

#define BOOST_PP_REPEAT_P(n)  BOOST_PP_CAT(BOOST_PP_REPEAT_CHECK_, BOOST_PP_REPEAT_ ## n(1, BOOST_PP_NIL BOOST_PP_TUPLE_EAT_3, BOOST_PP_NIL))

#define BOOST_PP_REPEAT_CHECK_BOOST_PP_NIL                1
#define BOOST_PP_REPEAT_CHECK_BOOST_PP_REPEAT_1(c, m, d)  0
#define BOOST_PP_REPEAT_CHECK_BOOST_PP_REPEAT_2(c, m, d)  0
#define BOOST_PP_REPEAT_CHECK_BOOST_PP_REPEAT_3(c, m, d)  0

/* tuple/eat.hpp */
#define BOOST_PP_TUPLE_EAT_3(e0, e1, e2)
とりあえず n == 2 を展開してみましょう。

BOOST_PP_REPEAT_P(2)
-> BOOST_PP_CAT(BOOST_PP_REPEAT_CHECK_, BOOST_PP_REPEAT_ ## 2(1, BOOST_PP_NIL BOOST_PP_TUPLE_EAT_3, BOOST_PP_NIL))
-> BOOST_PP_CAT(BOOST_PP_REPEAT_CHECK_, BOOST_PP_REPEAT_2(1, BOOST_PP_NIL BOOST_PP_TUPLE_EAT_3, BOOST_PP_NIL))
-> BOOST_PP_CAT(BOOST_PP_REPEAT_CHECK_, BOOST_PP_NIL BOOST_PP_TUPLE_EAT_3(z, 0, BOOST_PP_NIL))
-> BOOST_PP_CAT(BOOST_PP_REPEAT_CHECK_, BOOST_PP_NIL)
-> BOOST_PP_REPEAT_CHECK_ ## BOOST_PP_NIL
-> BOOST_PP_REPEAT_CHECK_BOOST_PP_NIL
-> 1
1に展開されました。次 n == 1 のとき。
これも同様に展開すると 1になります。
なるほど。BOOST_PP_REPEAT_P(2) == 1 && BOOST_PP_REPEAT_P(1) == 1 だから BOOST_PP_REPEAT_1 になるのか。

しかしちょっと待ってください。
BOOST_PP_REPEAT_P(1)の展開中にはBOOST_PP_REPEAT_1が出てきますよね。
これ2回目のループ(BOOST_PP_REPEAT_1の展開中)の場合展開できないはずです。
では展開できない場合をやってみましょう。

BOOST_PP_REPEAT_P(1)
-> BOOST_PP_CAT(BOOST_PP_REPEAT_CHECK_, BOOST_PP_REPEAT_ ## 1(1, BOOST_PP_NIL BOOST_PP_TUPLE_EAT_3, BOOST_PP_NIL))
-> BOOST_PP_CAT(BOOST_PP_REPEAT_CHECK_, BOOST_PP_REPEAT_1(1, BOOST_PP_NIL BOOST_PP_TUPLE_EAT_3, BOOST_PP_NIL))
-> BOOST_PP_REPEAT_CHECK_ ## BOOST_PP_REPEAT_1(1, BOOST_PP_NIL BOOST_PP_TUPLE_EAT_3, BOOST_PP_NIL)
-> BOOST_PP_REPEAT_CHECK_BOOST_PP_REPEAT_1(1, BOOST_PP_NIL BOOST_PP_TUPLE_EAT_3, BOOST_PP_NIL)
-> 0
なんとぉ!
0に展開されました。
n == 2, 3 の時も同様に BOOST_PP_REPEAT_n が展開できないとき BOOST_PP_REPEAT_P(n) は 0に展開されます。
なんて恐ろしいマクロなんだ。

でもまぁこれで2回目のループの時はBOOST_PP_REPEAT_P(2) == 1 && BOOST_PP_REPEAT_P(1) == 0 だから BOOST_PP_REPEAT_2 になりました。

2重ループの例は次のようになります。

#define MUL(z, n, _)    BOOST_PP_EXPR_IF(n, *) (BOOST_PP_REPEAT(BOOST_PP_INC(n), SUM, _))
#define SUM(z, n, _)    BOOST_PP_EXPR_IF(n, +) BOOST_PP_INC(n)

BOOST_PP_REPEAT(10, MUL, _);
-> BOOST_PP_REPEAT_1(10, MUL, _);
-> MUL(z, 0, _) MUL(z, 1, _) ... MUL(z, 9, _);
-> (BOOST_PP_REPEAT(1, SUM, _)) * (BOOST_PP_REPEAT(2, SUM, _)) * ... * (BOOST_PP_REPEAT(10, SUM, _));
-> (BOOST_PP_REPEAT_2(1, SUM, _)) * (BOOST_PP_REPEAT_2(2, SUM, _)) * ... * (BOOST_PP_REPEAT_2(10, SUM, _));
-> (1) * (1 + 2) * (1 + 2 + 3) * ... * (1 + 2 + ... + 10);
たしかに、これだと再帰してませんね。
※ ちなみに BOOST_PP_REPEAT は3重ループまで出来ます。がここまで読んだあなたなら4重目以降も実装できますよね :innocent:

BOOST_PP_REPEAT_n(count, macro, data)
最後の敵はこいつ BOOST_PP_REPEAT_n 。BOOST_PP_REPEAT の本体ですがこいつはそんなに強くないので大丈夫です。

repetition/repeat.hpp

#define BOOST_PP_REPEAT_1(c, m, d)    BOOST_PP_REPEAT_1_I(c, m, d)
#define BOOST_PP_REPEAT_1_I(c, m, d)  BOOST_PP_REPEAT_1_ ## c(m, d)

#define BOOST_PP_REPEAT_1_0(m, d)
#define BOOST_PP_REPEAT_1_1(m, d)     m(2, 0, d)
#define BOOST_PP_REPEAT_1_2(m, d)     BOOST_PP_REPEAT_1_1(m, d) m(2, 1, d)
#define BOOST_PP_REPEAT_1_3(m, d)     BOOST_PP_REPEAT_1_2(m, d) m(2, 2, d)
...
#define BOOST_PP_REPEAT_1_255(m, d)   BOOST_PP_REPEAT_1_254(m, d) m(2, 254, d)
#define BOOST_PP_REPEAT_1_256(m, d)   BOOST_PP_REPEAT_1_255(m, d) m(2, 255, d)
BOOST_PP_REPEAT_1は上記のようになってるので展開すると以下のようになります。

BOOST_PP_REPEAT_1(10, macro, data)
-> BOOST_PP_REPEAT_1_I(10, macro, data)
-> BOOST_PP_REPEAT_1_ ## 10(macro, data)
-> BOOST_PP_REPEAT_1_10(macro, data)
-> BOOST_PP_REPEAT_1_9(macro, data) macro(2, 9, data)
-> BOOST_PP_REPEAT_1_8(macro, data) macro(2, 8, data) macro(2, 9, data)
...
-> macro(2, 0, data) macro(2, 1, data) ... macro(2, 9, data) 
これまた圧倒的に単調な定義ですが、これで256まではループっぽいことが出来ました。

あと最初にzはループ最適化用だとか言ってましたがこれは次のループのnのことでした。
2重ループの例の MUL を

#define MUL(z, n, _)  BOOST_PP_EXPR_IF(n, *) (BOOST_PP_REPEAT_ ## z(BOOST_PP_INC(n), SUM, _))
のように定義すると BOOST_PP_AUTO_REC を使わずに BOOST_PP_REPEAT_2 を生成できるようになるのでプリプロセス時間が少し短縮されるようになります。

おわりに
どうでしたか?
これであなたもBOOST_PP大好き人間になりましたか?
BOOST_PP_FORとかのループ系のやつはほぼ同じような感じになっているのでもう読めるようになってると思います。:innocent:
あと特殊なのは BOOST_PP_ITERATE とか BOOST_PP_SLOT とかが残ってますがもうしんどいので勘弁してください。
この記事が読めたあなたならきっと自力で解読できるはずです。

あとBOOST_PPはPSoCととても相性がいいです。PSoCを使っている方は今すぐBOOST_PPを使うように。

展開できない例を展開できるようにしてみた
#define X(seq)             X_I0 seq
#define X_I0(elem, isEnd)  X_ ## isEnd(elem, 1)
#define X_I1(elem, isEnd)  X_ ## isEnd(elem, 0)
#define X_0(elem, next)    elem + X_I ## next
#define X_1(elem, next)    elem

X((a, 0)(b, 0)(c, 1))      // X開始
-> X_I0(a, 0)(b, 0)(c, 1)  // X_I0開始
-> X_0(a, 1)(b, 0)(c, 1)
-> a + X_I1(b, 0)(c, 1)    // X_I0終了, X_I1開始
-> a + X_0(b, 0)(c, 1)
-> a + b + X_I0(c, 1)      // X_I1終了, X_I0開始
-> a + b + X_1(c, 1)
-> a + b + c               // X_I0終了, X終了
X_I0, X_0は2回出てきてるけどX_I0の展開中にX_I0は出てきていない(途中で切れ目がある)のでOK

  
  
  
  
  
  
## PP 
https://qiita.com/hikarin522/items/5329647ec2a542653f68  
明日から使える boost/preprocessor/facilities/  https://dechimal.hatenadiary.com/entry/20101214/1292354432   
BOOST_PP_REPEAT  http://nekko1119.hatenablog.com/entry/2013/05/01/010105   
Boost.PP 闇魔術を触ってみた  http://www.slideshare.net/DtYaZsK/ss-32425592  
소개  http://cpplover.blogspot.com/2008/07/boostpreprocessor-meta-programming.html  
일본어 홈페이지  http://boost.cppll.jp/HEAD/libs/preprocessor/doc/index.html  
Boost::preprocessor 로 템플릿 코드 생성  http://rein.kr/blog/archives/486  
  http://www.kmonos.net/alang/boost/classes/preprocessor.html  
http://d.hatena.ne.jp/DigitalGhost/searchdiary?word=*[Preprocessor]&  
  http://slashdot.jp/~A7M/journal/481116  
  http://idlysphere.blog66.fc2.com/blog-entry-175.html  
  http://cpplover.blogspot.com/2008/07/blog-post_17.html  
  
  