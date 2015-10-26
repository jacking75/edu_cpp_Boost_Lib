
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
http://jacking.tistory.com/1070

### Ubuntu에서 Boost 라이브러리 빌드 테스트 해 보기
http://jacking.tistory.com/1079

### OpenSUSE에서 Boost 라이브러리 빌드하기
http://jacking.tistory.com/1173

### clang에서 boost 라이브러리 사용하기
http://jacking.tistory.com/1174

### Ubuntu Eclipse에서 C++ 빌드 하기
http://jacking.tistory.com/1086

### MonoDevelop에서 Boost.thread를 사용한 C++ 빌드하기
http://jacking.tistory.com/1080

## 참고
### Boost 라이브러리 빌드 링크 모음
http://jacking.tistory.com/1068

### Boost 라이브러리를 어떤 컴파일러 어떤 버전으로 빌드했는지 알고 싶을 때
http://jacking.tistory.com/974

### VC10, VC11 외부 라이브러리와 헤더 파일 디렉토리 설정
http://jacking.tistory.com/987
