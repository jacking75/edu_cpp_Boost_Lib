# boost::numeric::interval
  
## 개요: 구간 연산 라이브러리는
구간 연산 라이브러리는 말 그대로 수학의 "구간"을 다루기 위한 라이브러리이다.  
Boost 구간 연산 라이브러리는 `#include <boost/numeric/interval.hpp>` 를 포함하는 것만으로 간단하게 사용할 수 있다.   
```
#include <iostream>
#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>

int main () {
    boost::numeric::interval<int> a(-1, 2);
    std::cout << a << std::endl;
    return 0;
}
```
  
<pre>  
$ g ++ hello-interval.cpp -o hello-interval
$ ./hello-interval
[-1,2]
</pre>    
  
이처럼 구간의 기반이 되는 T 타입을 템플릿 인수에 구간 타입 boost::numeric::interval<T>을 사용할 수 있다.   
사실, 이 구간 타입 interval<T, Policies>과 2 개의 템플릿 인수를 취하도록 선언 되어 있고, Policies에 다양하게 지정하여 세세한 행동을 바꿀 수 있다.  
기본값은 기본 정책이 사용되고, 보통은 기본만으로 충분하다.  
  
Remark. 구간 연산 라이브러리는 어떤 계산기 · 어떤 컴파일러에 대해서도 "정확한 계산"을 지원하고 있는 것은 아니다. 사용하고 있는 컴퓨터 환경에서 구간 연산 라이브러리를 올바르게 사용할 수 있는지 여부는 공식 문서 등으로 확인 해야 한다.
  
  
  
## 사칙 연산과 기본 함수
  
### 구간끼리의 사칙 연산
부동 소수점 오차까지 고려하면서 구간의 수 모두에 대한 연산 결과를 모두 포함하도록 구간을 계산하여 반환한다.  
  
test-interval-arith-IRxIR.cpp:  
```
#include <iostream>
#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>

int main () {
    typedef double R;
    typedef boost::numeric::interval<R> IR;
    using std::cout;
    using std::endl;
    const IR a(1.0, 2.0);
    const IR b(3.0, 4.0);
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "a / b = " << a / b << endl;
    return 0;
}
```    
<pre>
a = [1,2]
b = [3,4]
a + b = [4,6]
a - b = [-3,-1]
a * b = [3,8]
a / b = [0.25,0.666667]
</pre>  
[Wandbox](https://wandbox.org/permlink/8qtcQb4XXNOkLEzw )  
    
또한 좌변 값에 사칙 연산을 직접 수행하는 연산자 +=, -=, *=, /= 도 있다.  
  
Remark. 일반적으로 구간의 이항 연산 +과 -는 서로 반대 연산의 관계는 되지 않는다. 예를 들어, 원래 오차가 없이 + 와 - 가 서로 반대 연산의 관계에 있는 int를 기반으로 하는 구간 타입 interval<int>의 경우를 생각해도 [1, 2] - [1, 2]는 [0, 0]는 안된다. * 와 / 도 마찬가지이다. 따라서, 일반적으로는 구간 연산의 반복으로 구간은 점점 커져간다. 부동 소수점 타입의 경우에는 구간 타입을 생각하여 원래 역 연산 관계에 없었던 사칙 연산이 가지는 '오차'라는 성격이 더 두드러 졌다고도 할 수 있다.  
    
  
### 구간 수의 사칙 연산
사칙 연산의 인수의 한쪽은 베이스 타입 T의 값으로 제공 할 수 있다. 이러한 경우 먼저 인수를 interval<T>로 변환하여 연산을 수행한 것과 같은 결과가 된다(이것은 이후 나오는 다른 함수에 대해서도 대략 마찬가지이다).  
Remark. interval<T>는 하단과 상단의 2 인자를 취하는 생성자 이외에도 베이스 타입 T의 값 x를 취하고, 일점 구간[x, x]를 만드는 1 인자 생성자가 있다.  
  
test-interval-arith-IRxR.cpp:  
```
#include <iostream>
#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>

int main () {
    typedef double R;
    typedef boost::numeric::interval<R> IR;
    using std::cout;
    using std::endl;
    const IR a(1.0, 2.0);
    const R b = 3.0;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "a / b = " << a / b << endl;
    return 0;
}
```  
  
  
### 숫자끼리의 사칙 연산
두 인수를 베이스 타입으로 주고 사칙하려면 어떻게할까? 이런 때에는 boost::numeric::interval_lib 에 정의된 함수 add, sub, mul, div을 사용한다. 이들은 베이스 타입 T 값을 2개의 인수로 잡고, 사칙 연산 절단 · 절상을 각각 하단 · 상단으로 하는 interval<T> 타입의 구간을 계산하여 반환한다.  
  
test-interval-arith-RxR.cpp:  
```
#include <iostream>
#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>

int main () {
    typedef double R;
    typedef boost::numeric::interval<R> IR;
    using std::cout;
    using std::endl;
    const R a = 1.7;
    const R b = 3.3;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    {
        using namespace boost::numeric::interval_lib;
        cout << "a + b = " << add<IR>(a, b) << "w(" << width(add<IR>(a, b)) << ")" << endl;
        cout << "a - b = " << sub<IR>(a, b) << "w(" << width(sub<IR>(a, b)) << ")" << endl;
        cout << "a * b = " << mul<IR>(a, b) << "w(" << width(mul<IR>(a, b)) << ")" << endl;
        cout << "a / b = " << div<IR>(a, b) << "w(" << width(div<IR>(a, b)) << ")" << endl;
    }
    return 0;
}
```
    
위의 프로그램에서 제대로 구간 연산이 이루어지면 아래와 같은 결과를 얻을 수 있다. 여기서 width는 구간의 폭, 즉 상단과 하단의 차이를 계산하는 함수이다. 부동 소수점 연산에 의해 부동 소수점 오차가 0이 아닌 구간 폭으로 나타나고 있음을 간파 할 수 있다.  
  
output-for-test-interval-arith-RxR.txt  
<pre>
a = 1.7
b = 3.3
a + b = [5,5] w (8.88178e-16)
a - b = [-1.6, -1.6] w (0)
a * b = [5.61,5.61] w (8.88178e-16)
a / b = [0.515152,0.515152] w (1.11022e-16)
</pre>  
  
Remark. 물론 1 인자 생성자를 사용하여 IR(1) / IR(9) 등과 써도 비슷한 것을 얻을 수 있지만 이러한한데 구간끼리의 연산이라고 같은 계산이 추가로 발생 해 버리는 순간 div<IR>(1, 9)이 약간 효율이 좋다.  
  

### 하단 · 상단 · 중간 · norm
구간의 폭을 계산하는 함수 width처럼 interval<T> 타입으로 주어진 구간에 대해서 하등의 베이스 타입 T 의 값을 반환하는 함수로 lower, upper, median, norm 가 있다. 처음 세개는 문자적인 의미로 각 구간의 하단 · 상단 · 중간 점을 반환하는 함수이다. norm이 구간이 포함 숫자의 최대 절대 값을 반환한다.




## 출처:
- https://qiita.com/t_uda/items/7712671389e016d24df6
- https://qiita.com/t_uda/items/e4f554b8af864b638a6d
- https://qiita.com/t_uda/items/abcf31a7b1787604a4f4
- https://qiita.com/t_uda/items/e08ca64b318917e6d4ab
