## Heap
- Boost.Heap의 컨테이너는 표준 컨테이너처럼 메모리 할당기 및 비교 함수를 커스터마이즈 할 수 있는 Policy-based Design에 근거하고 있다.  
- 표준 컨테이너는 이들 설정이 템플릿 매개 변수 순서에 의존하고 있지만 Boost.Heap의 경우는 명명된 템플릿 매개 변수를 채용하고 있으므로 지정하는 순서는 사용자가 마음대로 정할 수 있다.  
    - 우선 순위를 결정하기 위한 비교 함수를 커스텀마이즈 하고 싶다면 Boost.Heap컨테이너에 boost::heap::compare를 지정한다.  
      기본적으로 boost::heap::compare > 가 설정 되어 있다.
    - 메모리 할당기를 커스텀마이즈 하고 싶다면 Boost.Heap 컨테이너에 boost::heap::allocator를 지정한다.  
      기본적으로 boost::heap::compare > 가 설정 되어 있다.
    - Boost.Heap 컨테이너에서는 size() 멤버 함수가 기본적으로 상수 시간으로 계산된다.  
      이것은 크기를 위한 멤버 변수를 내부에 가지고 있는 것을 의미한다.  
      size() 멤버 함수에 상수 시간이 필요 없는 경우, boost::heap::constant_time_size를 지정하여 선형 시간으로 할 수 있다.  
      기본적으로 boost::heap::constant_time_size 이 설정된다.
  
```C++
// http://faithandbrave.hateblo.jp/entry/20120510/1336641311

#include <iostream>
#include <boost/heap/fibonacci_heap.hpp>

namespace heap = boost::heap;

int main ()
{
    heap::fibonacci_heap<
        int,
        heap::allocator<std::allocator<int>>,
        heap::compare<std::greater<int>>,
        heap::constant_time_size<false>
    > que;

    que.push(3);
    que.push(1);
    que.push(4);

    std::cout << "size: " << que.size() << std::endl;

    while (!que.empty()) {
        std::cout << que.top() << std::endl;
        que.pop();
    }
}
```
<pre>
size: 3
1
3
4
</pre>  
  
이 외에도 stable, mutable_, stability_counter_type, arity, store_parent_pointer 라는 설정이 있다.  
    
