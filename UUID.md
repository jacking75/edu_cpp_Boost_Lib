### 개용
범용적인 unique한 식별자.
16바이트로 표시하는 유니크한 값을 생성할 수 있다.

```
//출처: http://www.kmonos.net/alang/boost/classes/uuid.html
#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
using namespace boost::uuids;

int main()
{
	uuid u1 = random_generator()();
	uuid u2 = random_generator()();
	uuid u3 = random_generator()();
	std::cout << u1 << std::endl;
	std::cout << u2 << std::endl;
	std::cout << u3 << std::endl;

	uuid base = string_generator()("0123456789abcdef0123456789abcdef");
	std::cout << base << std::endl;

	name_generator gen(base);
	uuid u4 = gen("www.kmonos.net");
	uuid u5 = gen("boost.org");
	uuid u6 = gen("www.kmonos.net");
	std::cout << u4 << std::endl;
	std::cout << u5 << std::endl;
	std::cout << u6 << std::endl; 
}
```
