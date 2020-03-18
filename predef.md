## 개요
- Pre_defined Compiler Macros 프로젝트에서 모아 놓은 매크로로 C++ 컴파일 환경이나 타겟 환경(OS) 정보를 얻을 수 있다.  
- Boost 1.55 에서 추가  
  
  
  
## OS, 컴파일러, 비주얼스튜디오 버전
  
```
// 출처: http://d.hatena.ne.jp/osyo-manga/20131213/1386933884
#include <boost/config.hpp>
#include <boost/predef.h>
#include <iostream>

int main()
{
  std::cout << BOOST_COMPILER << std::endl;

#if BOOST_OS_WINDOWS
  std::cout << "OS is Windows." << std::endl;
#else
  std::cout << "OS is not Windows." << std::endl;
#endif

#if BOOST_COMP_CLANG
  std::cout << "Compiler is Clang." << std::endl;
#elif BOOST_COMP_MSVC
  std::cout << "Compiler is MSVC." << std::endl;
#else
  std::cout << "Compiler is unknow." << std::endl;
#endif


#if BOOST_COMP_MSVC && BOOST_COMP_MSVC >= BOOST_VERSION_NUMBER(12, 0, 0)
  std::cout << "VC 12" << std::endl;
#elif BOOST_COMP_MSVC && BOOST_COMP_MSVC >= BOOST_VERSION_NUMBER(13, 0, 0)
  std::cout << "VC 13" << std::endl;
#endif

  return 0;
}
```
