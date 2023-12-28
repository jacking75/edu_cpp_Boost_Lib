# 개요
- 날짜 계산 라이브러리.
- Gregorial을 사용한다.


# 사용 예  
  
## 월의 마지막 날
```
#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>

int main()
{
    using namespace boost::gregorian;

    const int day = gregorian_calendar::end_of_month_day(2011, 2);
    std::cout << day << std::endl;
}
// 출처: https://sites.google.com/site/boostjp/tips/date_time
```

## 날짜 더하기/빼기
- 년의 가감산에는 years 타입,  월의 가감산에는 months 타입, 날의 가감산에는 days 타입을 사용한다.

```
#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::gregorian;

int main()
{
    const date d1(2011, Apr, 1);
    const date d2 = d1 + months(1) - days(1);

    std::cout << to_simple_string(d2) << std::endl;
}
//출처: https://sites.google.com/site/boostjp/tips/date_time
```

## UTL <-> LOCAL
```
#include <iostream>

#include <boost/date_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/local_time_adjustor.hpp>

int main(const int argc, const char * const argv[])
{
	using namespace boost;

	posix_time::ptime now = posix_time::second_clock::universal_time();

	// utc to local 1
	date_time::c_local_adjustor<posix_time::ptime> adj1;
	posix_time::ptime t1 = adj1.utc_to_local(now);

	// utc to local 2
	date_time::local_adjustor<posix_time::ptime, 9, posix_time::no_dst> adj2;
	posix_time::ptime t2 = adj2.utc_to_local(now);

	// local to utc
	posix_time::ptime t3 = adj2.local_to_utc(t2);

	// out
	std::cout << now << std::endl;
	std::cout << t1 << std::endl;
	std::cout << t2 << std::endl;
	std::cout << t3 << std::endl;

	return 0;
}
```
