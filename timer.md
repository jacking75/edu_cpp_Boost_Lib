
### 개요
- 표준 라이브러리 clock()을 사용하여 간단하게 시간 계측을 할 때 사용하면 편리한 클래스


### 기본 사용 방법

```
// 출처: http://tips.hecomi.com/entry/20100715/1279210634
#include <iostream>
#include <boost/timer.hpp>
#include <windows.h>

int main()
{
	boost::timer t; // 타이머 시작
	Sleep(1234);
	std::cout << t.elapsed() << " sec" << std::endl;	// 1.234 sec
	t.restart(); // 타이머 리셋
	Sleep(2345);
	std::cout << t.elapsed() << " sec" << std::endl;	// 2.344 sec
	return 0;
}
```
elapsed 에서 경과한 시간을 double 타입으로 얻을 수 있다. restart 하면 타이머가 리셋되어 다시 0에서 카운트 된다.
