
## Windows
### 소스를 다운로드 받은 직접 빌드
대부분의 boost 라이브러리는 원하는 기능의 헤더 파일을 포함하면 사용할 수 있지만 OS의 기능을 사용하는 것들은 빌드를 해야한다.

1. boost 라이브러리(여기서는 1.50)을 다운로드 한다    
2. 'E:\Dev\CommonLib\boost_1_50_0' 디렉토리에 압축을 푼다.
3. 먼저 빌드 실행 파일을 만들기 위해서 위의 디렉토리에서 bootstrap.bat 를 실행한다.
<img src="resource\072012_1617_boost1.png">

4. 디렉토리에 b2.exe와 bjam.exe 파일 두개가 만들어졌다.
이중 bjam.exe를 사용하여 빌드를 하겠다. 참고로 예전에는 bjam.exe를 사용 하였으나 근래에는 b2.exe로 넘어가는 추세이다.
<img src="resource\072012_1617_boost2.png">
<img src="resource\072012_1617_boost3.png">
내 컴퓨터에는 VC10과 VC11(RC 버전)이 있는데 기본적으로 가장 최신 VC를 대상으로 빌드를 하는 것 같다.
물론 옵션을 지정하면 VC 버전이나 64비트, 빌드 출력 디렉토리도 지정할 수 있다.
<img src="resource\072012_1617_boost4.png">

옵션으로 디렉토리를 지정하지 않으면 라이브러리 파일은 stage\lib 디렉토리에 생성된다.


#### '런타임 라이브러리 정적 링크' 라이브러리 생성하기
- 위의 방식으로 하면 '런타임 라이브러리 정적 링크' 라이브러리(빌드 후 만들어지는 lib 파일에 -sgd 가 붙는)가 만들어 지지 않는다.
그래서 다시 b2.exe 파일을 사용하여 빌드를 했다.
- b2.exe 명령어를 사용할 때 옵션으로 -prefix (빌드 라이브러리가 생성될 디렉토리)를 붙이지 않으면 이상한 곳에 만들어질 수 있으니 꼭 붙여야 한다.
나는 stage/lib 디렉토리에 만들어지게 하고 싶어서 다음과 같이 했다.
<pre>
b2 -prefix=.\stage\lib --toolset=msvc runtime-link=static
</pre>

  예)
  <pre>
  VS 2005
  /MT, /MTd
  b2 -prefix=.\stage\lib --toolset=msvc-8.0 runtime-link=static

  /MD, /MDd
  b2 -prefix=.\stage\lib --toolset=msvc-8.0 runtime-link=shared
  </pre>

  <pre>
  VS 2012
  /MT, /MTd
  b2 --stagedir=stage --toolset=msvc-11.0 runtime-link=static

  /MD, /MDd
  b2 --stagedir=stage --toolset=msvc-11.0 runtime-link=shared

  64비트
  /MT, /MTd
  b2 --stagedir=stage64 address-model=64 --toolset=msvc-11.0 runtime-link=static

  /MD, /MDd
  b2 --stagedir=stage64 address-model=64 --toolset=msvc-11.0 runtime-link=shared
  </pre>


### VC용으로 빌드된 Boost 라이브러리 얻기
- VC용으로 이미 빌드된 Boost 라이브러리 Lib 파일을 얻을 수 있다.
- 위치는 http://sourceforge.net/projects/boost/files/boost-binaries/  이다.
- 다양한 Boost 라이브러리 버전 및 VC 버전별로 준비 되어 있다.
<img src="resource\vc_lib_down.png">



## Linux
### linux는 OpenSUSE 12.3 64Bit, Boost 라이브러리 1.53 버전 빌드
- http://warmz.tistory.com/904 의 Mac에서 빌드 방법을 참고했다.
1. Boost 라이브러리 다운로드 후 압축 풀기
2. 터미널에서 압축을 푼 디렉토리로 이동 후 아래처럼 입력
 $ ./bootstrap.sh
3. 생성된 b2를 사용하여 Boost 라이브러리 빌드하기
 $ ./b2 toolset=gcc link=static threading=multi address-model=64

빌드가 끝나면 Boost 라이브러리 디렉토리 안의 stage/lib 디렉토리에 빌드 릴리즈용 lib 파일 생성.
<pre>
./b2 toolset=gcc link=static threading=multi address-model=64 variant=debug
./b2 toolset=gcc link=static threading=multi address-model=64 variant=release
</pre>

### b2의 옵션 설명
- 출처: https://sites.google.com/site/boostjp/howtobuild
- toolset
<pre>
하나의 머신에 서로 다른 종류의 컴파일러가 인스톨 되어 있는 경우는 toolset 명령어로 지정할 수 있다. 예를 들면:
borland  : Borland사의 컴파일러
dmc          : Digital Mars사의 컴파일러
darwin    : Apple에서 손을 댄 gcc 컴파일러(Mac OS)
gcc            : GNU 프로젝트에 의한 컴파일러
intel         : Intel사 컴파이럴
msvc         : Microsoft사 컴파일러
</pre>
로 지정할 수 있다. msvc-9.0 (Visual C++ 2008), msvc-10.0 (Visual C++ 2010)와 같이 버전 지정도 할 수 있다.

- link
    - 이것은 static, shared 라이브러리를 만들지를 지정하는 명령어
    - link=static,shared
와 같이 사용한다.
<pre>
lib, dll (Windows)
a, dylib (Mac OSX)
a, so (Other Systems)
와 같은 라이브러리 파일을 생성한다.
</pre>

- threading
    - multi: 멀티 스레드 라이브러리 생성
    - single: 싱글 스레드 라이브러리 생성.

- variant
    - debug: 디버그 빌드를 생성
    - release: 릴리즈 빌드를 생성

### 참고 글
- [(한글)Install boost library for Linux](http://gilgil.net/?document_srl=7273)
- LLVM
<pre>
./b2 toolset=clang linkflags="-stdlib=libc++" cxxflags="-std=c++11 -stdlib=libc++"
</pre>
- [boost 1.54.0 library 설치](http://pururoong.tistory.com/36)


### Ubuntu에서 Boost 라이브러리 빌드 테스트 해 보기
- Boost 라이브러리는 home에서 아래의 디렉토리에 설치
<img src="resource\ubuntu_boost_build-1.PNG">

- Boost 라이브러리를 빌드하여 debug, release 각각 만든다(정적 라이브러리).
<img src="resource\ubuntu_boost_build-2.PNG">

- tesp.cpp와 Makefile을 TestCppBoost 디렉토리에 생성
<img src="resource\ubuntu_boost_build-3.PNG">

- 소스코드, Makefile
<img src="resource\ubuntu_boost_build-4.PNG">

 <img src="resource\ubuntu_boost_build-5.PNG">

- 빌드 후 실행
<img src="resource\ubuntu_boost_build-6.PNG">


### OpenSUSE에서 Boost 라이브러리 빌드하기
리눅스에서는 기본 옵션이 아래처럼 된다.
<pre>
link=static,shared threading=multi variant=release
</pre>

본인은 아래와 같이 빌드했다.
(OpenSUSE가 64비트라 그런지 기본으로 64비트로 빌드 되었다.
<pre>  
./bootstrap.sh
./b2 toolset=clang link=static,shared cxxflags="-std=c++11 -stdlib=libstdc++" linkflags="-stdlib=libstdc++" threading=multi
</pre>


### clang에서 boost 라이브러리 사용하기
- 소스 코드: hello-boost.cpp

```
#include <boost/regex.hpp>
#include <iostream>
#include <string>


int main()
{
	std::string line;
	boost::regex pat("^Subject: (Re: |Aw:)*(.*)");

	while(std::cin)
	{
		std::getline(std::cin, line);
		boost::smatch matches;

		if(boost::regex_match(line, matches, pat))
			std::cout << matches[2] << std::endl;
	}

	return 0;
}
```

- Makefile

```
CXX=clang++
CXXFLAGS=-I/home/dev/Dev/C++/boost
LDFLAGS=-L/home/dev/Dev/C++/boost/stage/lib
LDLIBS=-lboost_regex

all:hello-boost

clean:
	rm -rf hello-boost
	rm -rf *.o
```


### Ubuntu Eclipse에서 C++ 빌드 하기
- 리눅스 플랫폼에서 C++ 프로그래밍을 쉽게 하는 방법은 Eclipse라는 IDE를 사용하면 좋다.
그러나 VS를 사용했던 프로그래머로서는 사용해 보면 초반부터 간단한 부분에서 막혀서 스트레스를 받는데 이중 몇 가지를 기록 차원에서 간단하게 정리한다. 로젝트 설정을 위해서는 아래의 설정 창을 열어야 한다.

 <img src="resource\Eclipse_boost-1.PNG">

- C++11 사용하기. 아래 처럼 -std=c++11 을 입력한다(참고로 gcc 4.8 사용)
<img src="resource\Eclipse_boost-2.PNG">
    - [Eclipse 에서 MinGW GCC 컴파일러로 C++11 사용하기](http://blog.powerumc.kr/404)


- 외부 해더 파일 설정
<img src="resource\Eclipse_boost-3.PNG">


- 외부 lib 파일 설정
<img src="resource\Eclipse_boost-4.PNG">

- 여기서 주의할 점은 아래와 같이 이름이 libboost_reggex.a 라는 lib 파일을 설정에 등록할 때는 앞의 'lib'과 뒤의 '.a'를 제외하고 입력해야 한다.
<img src="resource\Eclipse_boost-5.PNG">
    - [eclipse에서 boost 사용하는데 에러가 난다](http://abipictures.tistory.com/821)
    - How to Install, Build and Use the Boost C++ libraries for development with the Eclipse IDE using the MinGW compiler on a Windows Platform
http://theseekersquill.wordpress.com/2010/08/24/howto-boost-mingw/


- std::thread 사용. 아래처럼 '-pthread' 를 입력한다.
<img src="resource\Eclipse_boost-6.PNG">
    - [How to make CDT/Eclipse work with C++11 threads?](http://stackoverflow.com/questions/10395936/how-to-make-cdt-eclipse-work-with-c11-threads)


### MonoDevelop에서 Boost.thread를 사용한 C++ 빌드하기
예전에는 Windows의 개발자가 Linux에서 C++ 프로그램을 개발하는 것이 쉽지 않았다. 쉽지 않은 이유는 Visual C++과 같은 툴이 없기 때문이었다.
그러나 요즘은 이클립스나 KDevelop, MonoDevelop를 사용할 수 있어서 예전 보다 엄청 좋아졌다.
MonoDevelop를 사용하여 Boost 라이브러리의 thread 라이브러리를 사용한 C++ 프로그램을 빌드하는 방법을 간단하게 설명한다.
컴파일러는 g++을 사용하고, Boost 라이브러리는 빌드 된 상태이다.
MonoDevelop를 사용하여 C++ 프로젝트를 만든 후 아래의 코드를 코딩한다.
<img src="resource\mono_boost-1.PNG">

아래 그림의 팝업 메뉴에서 'Option'을 선택하여 프로젝트 설정을 한다.
<img src="resource\mono_boost-2.PNG">

Boost 라이브러리의 헤더 파일과 lib 파일의 디렉토리를 추가한다
<img src="resource\mono_boost-3.PNG">

사용할 lib 파일을 추가한다
<img src="resource\mono_boost-4.PNG">

Boost 라이브러리의 디렉토리는 다음과 같다
<img src="resource\mono_boost-5.PNG">

스레드를 사용하므로 옵션으로 -pthread 를 추가한다
<img src="resource\mono_boost-6.PNG">

이제 빌드하면 된다!!!



## Boost 라이브러리를 어떤 컴파일러 어떤 버전으로 빌드했는지 알고 싶을 때
BOOST_COMPILER 라는 지시어를 사용하면 컴파일러 종류와 버전을 알려줍니다.
```
#include <iostream>
#include <boost/config.hpp>

int main()
{
    std::cout << BOOST_COMPILER << std::endl;
    return 0;
}
```


## Boost 라이브러리 빌드 링크 모음
### Windows
- [Boost 라이브러리 빌드 및 설치 - VC++ (VS2012, Boost 1.53)](http://warmz.tistory.com/903)
- [boost 간단 설치기 (Windows환경)  - VS 2010 중심](http://blog.naver.com/ykish/100188390123)
- [Boost 64비트로 빌드하기](http://blog.naver.com/lunu/100160768950)
- [VS2008에서 Boost 빌드하기](http://blog.naver.com/dbwls0105/30069044946)

### Mac
- [Boost 라이브러리 빌드 및 설치 - Mac](http://warmz.tistory.com/904)

### Linux
- [(한글)Install boost library for Linux](http://www.gilgil.net/7273)

### Android
- [Android NDK를 사용한 boost library 빌드 방법](http://imhallower.blog.me/90169489876)

### 일본 Boost 라이브러리 사이트의 빌드 글
https://sites.google.com/site/boostjp/howtobuild



## VC10, VC11 외부 라이브러리와 헤더 파일 디렉토리 설정
VC9까지는 '도구' – '옵션' 메뉴를 통해서 VC++ 디렉토리를 설정하였습니다.
이렇게 설정한 정보는 모든 VC++ 프로젝트에 적용됩니다.
그러나 VC10부터는 각 프로젝트 별로 VC++ 디렉토리를 설정합니다.
<img src="resource\VC10_setting-1.png">

각 프로젝트 마다 독립적으로 설정을 할 수 있어서 편한 부분도 있지만 때로는 모든 프로젝트에 공통적으로 적용되어야 하는 경우는 매번 설정하는 것이 귀찮을 수 있습니다.
(예로 DirectX나 boost 라이브러리 등)
이런 경우 '속성 매니저'를 통해서 VC++ 디렉토리를 설정하면 모든 프로젝트에 공통적으로 적용할 수 있습니다.
1. 일단 아무 프로젝트 하나를 열어 놓습니다.
2. 메뉴의 '보기' -> '속성 관리자'를 선택합니다.
<img src="resource\VC10_setting-2.png">

3. 속성 관리자에서 'Microsoft.Cpp.Win32.user'를 더블 클릭해서 열어 놓습니다.
<img src="resource\VC10_setting-3.png">
여기서 설정한 정보는 모든 프로젝트에 공통적으로 적용됩니다.
