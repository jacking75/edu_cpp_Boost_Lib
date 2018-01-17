## static_vector
- 1.54에 추가 되었다.
- std::vector와 std::array 양쪽의 특성을 모두 가진 하이브리드 컨테이너.
- 정적인 영역을 사용하는 가변 길이 배열.static_vector<T,N> 에서 N 이하의 가변 길이 배열이다.
- 고정 capacity를 두 번째 템플릿 인수로 지정하면 그 사이즈가 정적으로 영역을 확보한다. 
- 정해진 capacity를 넘지 않도록 추가/삭제를 해야 한다.
- 정해진 capacity를 넘지 않도록 추가/삭제를 해야 한다.범위를 넘으면 예외 발생
- 임의접근 가능.
- 제일 뒤 삽입/삭제는 정수 시간
- 선두나 중간 삽입/삭제는 선형 시간
- 최대 요소 수가 다른 static_vector<T, M>과 static_vector<T, N> 간에 복사를 할 수 있지만 target(좌변)의 capacity()가 source(우변) size() 보다 작으면 std:bad_alloc 예외 발생
- 할당자를 가지지 않는 것만 빼고는 std::vector와 같다.
    
  
### 예제 코드
```C++
// http://faithandbrave.hateblo.jp/entry/20130712/1373614497

#include <iostream>
#include <boost/container/static_vector.hpp>

int main()
{
    boost::container::static_vector<int, 3> v;

    // 요소 추가
    v.push_back(3);
    v.push_back(1);
    v.push_back(4);

    // 모든 요소 출력
    for (int x : v) {
        std::cout << x << std::endl;
    }

    // capacity를 넘는 요소를 추가하려고 하면 bad_alloc 예외가 던져진다
// v.push_back(5);
}
```
<pre>
3
1
4
</pre>  
    
  
```C++
// http://codezine.jp/article/detail/8259
#include <iostream>
#include <boost/container/static_vector.hpp>

using namespace std;

int main() {
  const int N = 10;

  cout << "static_vector<short," << N   << ">\t: " << sizeof(boost::container::static_vector<short,N>) << endl;
  cout << "static_vector<int,"   << N   << ">\t: " << sizeof(boost::container::static_vector<int,N>)   << endl;
  cout << "static_vector<int,"   << N*2 << ">\t: " << sizeof(boost::container::static_vector<int,N*2>) << endl;
  cout << endl;

  boost::container::static_vector<int,N> sv { -3, -2, -1 }; 

  try {
    for ( int i = 0; i < N; ++i ) {
      sv.emplace_back(i);
    }
  } catch ( std::exception& ex) {
    cout << ex.what() << endl;
    cout << "CAN'T INSERT ITEMS ANY MORE!\n" << endl;
  }

  for ( int item : sv ) {
    cout << item << endl;
  }

}
```  
  
   
### 예제 프로그램  
- static_vector_01
- static_vector_02
  
