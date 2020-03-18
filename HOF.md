# Boost.HOF
Boost 1.67.0에서 [Boost.HOF](https://github.com/boostorg/hof ) 라는 라이브러리가 추가 되었다.  
HOF은 Higher Order Functions( 고차 함수 )의 약자로, 함수 또는 함수 객체 유틸리티를 제공하는 라이브러리이다. 
header-only로 C++11 이상의 환경에서 사용할 수 있다.   
  
3개의 구성 요소 가 포함되어 있습니다.  
- Function Adaptors and Decorators: 기존의 기능을 강화하는 어댑터와 데코레이터
- Functions : 특정 목적을 위한 함수
- Utilities : 함수 정의 및 사용 시 사용할 범용 유틸리티
  
  
  
## Function Adapters and Decorators
  
### Funcition Adaptors
Function Adapters는 함수를 받고, 이 함수를 바탕으로 새로운 함수를 반환한다.  
  
#### lazy
lazy는 std::bind 처럼 사용할 수있는 어댑터이다.  
std :: bind와 다른 것은 constexpr에서도 이용할 수 있다는 점이다.  
샘플은 add의 제 2인자를 3으로 속박한 함수를 반환한다.  
```
#include <boost/hof.hpp>
namespace hof = boost::hof;

constexpr int add(int a, int b) {
  return a + b;
}

int main() {
  constexpr auto addThree = hof::lazy(add)(hof::_1, 3);
  static_assert(addThree(5) == 8);
}
```
  
  
#### pipable
pipable는 f(x, args...) 같은 함수를 x | f(args...) 형태로 부를 수 있도록한다.  
C++ 에서 `operator.` 는 오버로드 할 수 없지만 free 함수를 멤버 함수처럼 하는 이미지이다.  
(이미지 x(f)-> x.f()) 메소드 체인을 깨끗이한다.  
( g(h(f(x)) ) 같은 호출이 x | f() | h() | g() 같이 변형 할 수 있다) 샘플은 added 좌변이 add의 제 1인자이다.  
```
#include <boost/hof.hpp>
namespace hof = boost::hof;

constexpr int add(int a, int b) {
  return a + b;
}

int main() {
  constexpr auto added = hof::pipable(add);
  constexpr auto result = 3 | added(5) | added(7);
  static_assert(result == 15);
}
```
  
  
  
#### flow
flow는 여러 함수를 받고, 이 함수를 차례로 호출하는 함수를 반환한다.  
하나 전의 함수의 반환 값이 다음 함수의 인수이다.  
사용법에 따라서는 pipable와 비슷할 수 있다.  
샘플은 인수로 전달된 문자열을 html 태그로 둘러싸고 있다.  
```
#include <boost/hof.hpp>
#include <iostream>
#include <string>
namespace hof = boost::hof;

std::string addpre(std::string const& str) {
  return "<pre>" + str + "</pre>";
}

std::string adddiv(std::string const& str) {
  return "<div>" + str + "</div>";
}

int main() {
  auto wrap = hof::flow(addpre, adddiv, adddiv);
  std::cout << wrap("<span>aaa</span>") << std::endl;
}
```
  
출력  
<pre>
<div><div><pre><span>aaa</span></pre></div></div>
</pre>
  
  
  
#### infix
infix는 2개의 인수를 취하는 함수를 연결(중간 기법 모두)에 쓸 수 있도록 한다.  
a + b의 + 같은 이항 연산자의 연산자 위치 이미지이다.  
구체적으로는 `f(x, y)` 을 `x <f> y` 형식으로 부를 수 있도록 한다. 
샘플은 std::equal_to을 중간 기법으로 부를 수 있도록 한다.  
```
#include <boost/hof.hpp>
#include <functional>
namespace hof = boost::hof;

int main() {
  constexpr auto equals = hof::infix(std::equal_to<>{});
  constexpr auto a = 42;
  constexpr auto b = 42;
  static_assert(a <equals> b);
}
```
  
  
  
#### proj
proj는 proj의 제 2인자로 전달된 함수를 호출 할 때 각 인수에 첫 번째 인수로 전달된 함수를 호출하고 나서 전달한다.  
`proj(p, f)(1, 2, 3) == f(p(1), p(2), p(3))` 같은 이미지이다.  
```
#include <boost/hof.hpp>
#include <iostream>
namespace hof = boost::hof;

int twice(int x) {
  return x * 2;
}

void print(int x, int y, int z) {
  std::cout << x << " " << y << " " << z;
}

int main() {
  constexpr auto twiced_print = hof::proj(twice, print);
  twiced_print(5, 7, 11);
}
```
  
출력  
<pre>
10 14 22
</pre>
  
  
  
### Decorators
Decoratorssms 함수를 받고, Function Adapter를 반환하는 함수이다.  
  
#### repeat
repeat는 지정된 횟수만큼 함수 호출을 중첩한다.  
샘플은 초기 값 1에서 2배하는 함수를 10번 호출한다.  
```
#include <boost/hof.hpp>
namespace hof = boost::hof;

constexpr int twice(int x) {
  return x * 2;
}

int main() {
  constexpr auto pow_10 = hof::repeat(10)(twice);
  static_assert(pow_10(1) == 1024);
}
```
  
  
  
#### if_
if는 제 1 인수에 조건 메타 함수를 가지고, 이것이 true_type인 경우 제 2인수로 전달된 함수를 호출하는 함수를 반환한다.  
false_type의 경우 함수 호출하면 컴파일 오류가 되도록한다.  
샘플의 주석을 제거하면 컴파일 오류가 발생한다. true의 경우는 twice가 그대로 호출된다.  
```
#include <boost/hof.hpp>
#include <type_traits>
namespace hof = boost::hof;

constexpr int twice(int x) {
  return x * 2;
}

int main() {
  hof::if_(std::bool_constant<true>{})(twice)(1);
  // hof::if_(std::bool_constant<false>{})(twice)(1);
}
```
  
  
  
## Functions and Utilities
기타 유용한 기능을 소개한다  
  
### always
always는 인수로 취한 값을 반환하는 함수를 반환한다.  
이 함수는 임의의 인수를 취할 수 있지만 반환 값은 고정이다. 샘플은 항상 1을 반환하는 함수를 작성하고 있다.  
```
#include <boost/hof.hpp>
namespace hof = boost::hof;

int main() {
  constexpr auto one = hof::always(1);
  static_assert(one() == 1);
  static_assert(one(1, 3.14, "hello") == 1);
}
```
  
  
  
### identity
identity는 인수로 전달된 값을 그대로 반환하는 함수이다.  
```
#include <boost/hof.hpp>
namespace hof = boost::hof;

int main() {
  static_assert(hof::identity(42) == 42);
}
```
  
  
  
### construct
construct는 템플릿 매개 변수에 전달된 형식의 인스턴스를 작성하는 팩토리 함수를 반환한다.  
이 함수의 인자 값을 생성자의 인수로 전달한다.  
타입뿐만 아니라 템플릿 템플릿 매개 변수를 전달할 수 있으며, 이 경우 팩토리 함수의 인수에서 템플릿 매개 변수가 결정된다.  
샘플은 X 타입과 클래스 템플릿 Y의 인스턴스를 생성한다.  
```
#include <boost/hof.hpp>
namespace hof = boost::hof;

struct X {
  X(int, double, char const*) {}
};

template <typename T, typename U, typename V>
struct Y {
  Y(T, U, V) {}
};

int main() {
  const auto createX = hof::construct<X>();
  createX(1, 3.14, "hello");

  const auto createY = hof::construct<Y>();
  createY(true, nullptr, 2.f);
}
```
  
  
  
## BOOST_HOF_LIFT
BOOST_HOF_LIFT는 함수 템플릿 또는 함수 객체 템플릿과 오버로드 함수를 제네릭 람다에 래핑한다.  
일반적으로 함수 템플릿은 다른 고차 함수에 전달하는 경우 타입을 명시하고 인스턴스화 해야한다.  
오버로드 함수도 마찬가지로 어떤 오버로드를 전달 하는가를 캐스팅 해야한다.  
  
이미지 예  
```
std::vector< int > v;
```  
가 있다며 함수 템플릿의 경우  
```
template <typename T>
T my_add(T a, T b);
...
std::accumulate(v.begin(), v,end(), 0, &my_sum<int>);
```
  
오버로드 함수의 경우  
```
int my_add(int a, int b);
double my_add(double a, double b);
std::string my_add(std::string a, std::string b);
...
std::accumulate(v.begin(), v.end(), static_cast<int (*)(int, int)>(my_add));
```
  
같은 것이 필요하다.  
BOOST_HOF_LIFT는 제네릭 람다로 래핑 해준다.  
`[my_add](auto&&... args) { return my_add(std::forward<decltype(args)>(args...); }` 같은 이미지이다.  
  
함수 템플릿도 오버로드 함수를 아래와 같이 쓸 수 있다.  
```
std::accumulate(v.begin(), v.end(), BOOST_HOF_LIFT(my_add));
```
  
  
  
### tap
tap은 pipable에서 사용 되는 것을 상정하고 있다. pipable의 중간 값을 볼 수 있다.  
```
#include <boost/hof.hpp>
#include <iostream>
namespace hof = boost::hof;

constexpr int add(int a, int b) {
  return a + b;
}

template <typename T>
void print(char const* key, T const& value) {
  std::cout << key << ": " << value << std::endl;
}

int main()
{
  auto added = hof::pipable(add);
  auto result = 3 | added(5) | hof::tap([](auto value) { print("temp", value); }) | added(7);
  print("result", result);
}
```
  
출력  
<pre>
temp: 8
result: 15
</pre>
  
  
  
## 참고 
- [Boost.HOFについて](https://nekko1119.hatenablog.com/entry/2018/08/24/010924)