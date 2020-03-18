## boost::algorithm::split(&Result, &Input, Pred, eCompress = token_compress_off)
Pred(구분자) 문자열로 문자열 Input을 분해하고, 결과를 Result에 넣는다.  
Pred에는 공백 구분이라면 boost::algorithm::is_space(), (복수의)1 문자 구분자를 사용한다면 boost::algorithm::is_any_of()를 사용한다.  
빈 문자열을 절약하기 원한다면 boost::algorithm::token_compress_on을 지정.  
default로 boost::algorithm::token_compress_off이 지정되어 있다.  
  
```
#include<boost/algorithm/string.hpp>
namespace ba = boost::algorithm;

string original = " \tABC DEFG HIJKL MNOPQ RSTU \t VWXYZ \t";

vector<string> res;
cout << "split(res,original,is_space())\n\t→";
ba::split( res, original, ba::is_space() );
for(size_t i=0; i < res.size(); ++i) cout << "[" << res[i] << "]"; cout<<endl;

cout << "split(res,original,is_any_of(\" \\tU\"))\n\t→";
ba::split( res, original, ba::is_any_of(" \tU") );
for(size_t i=0; i < res.size(); ++i) cout << "[" << res[i] << "]"; cout<<endl;

cout << "split(res,original,is_any_of(\" \\tU\"),token_compress_on)\n\t→";
ba::split( res, original, ba::is_any_of(" \tU"), ba::token_compress_on );
for(size_t i=0; i < res.size(); ++i) cout << "[" << res[i] << "]"; cout<<endl;
```
   
결과  
<pre>
split(res,original,is_space())
        →[][][ABC][DEFG][HIJKL][MNOPQ][RSTU][][][VWXYZ][][]
split(res,original,is_any_of(" \tU"))
        →[][][ABC][DEFG][HIJKL][MNOPQ][RST][][][][VWXYZ][][]
split(res,original,is_any_of(" \tU"),token_compress_on)
        →[][ABC][DEFG][HIJKL][MNOPQ][RST][VWXYZ][]
</pre>
  
  
  
## boost::algorithm::join(const &Input, const &Separator)
문자열 시퀸스(string 배열이나 vector) Input을 Separator 문자열을 끼워 넣으면서 연결  
```
string splited[] ={"join", "test", "!!!"};
cout << ba::join( splited, "__" ) << endl;
```
  
결과  
<pre>
join__test__!!!
</pre>
  
  
  
## 문자열 치환 boost::algorithm::replace_all_copy
예제 코드  
```
#include <boost/algorithm/string.hpp>
#include <iostream>

int
main(){
    std::string source = "homuhomuhomu";
    
    // 결과를 복사해서 반환한다
    auto result1 = boost::algorithm::replace_all_copy(source, "homu", "mado");
    std::cout << result1 << std::endl;
    std::cout << source << std::endl;

    
    // 파괴적으로 변경한다
    boost::algorithm::replace_all(source, "homu", "mado");
    std::cout << source << std::endl;

    return 0;
}
```
  
결과  
<pre>
madomadomado
homuhomuhomu
madomadomado
</pre>
  
  
  
## string_ref
- Boost 라이브러리 1.53에서 추가된 라이브러리로 문자열의 소유권은 없으며 다만 선두 포인터와 크기만을 가진 문자열 클래스 이다.
- 이 라이브러리를 사용하면 좋은 예는 아래와 같다.
  
```
char szName[16];
void CheckName(const std::string& strName );
....
CheckName( szName );
```
  
위의 코드에서는 CheckName에 chat 타입의 문자열을 넘기면 임시적으로 string 오브젝트를 생성한다. CheckName 함수를 호출할 때 마다 불필요한 비용이 발생한다.  
만약 아주 빈번하게 CheckName가 호출된다면 임시 오브젝트 생성과 삭제에 대한 비용이 꽤 문제가 될 수 있다.  
  
바로 이런 부분에서 boost::string_ref를 사용하면 좋다.  
```
void CheckName(const boost::string_ref& strName );
```
  
로 바꾸면 szName을 담기 위해 저장소를 만들 필요 없이 szName에 대한 포인터와 문자 길이만을 가진 오브젝트를 만들기 때문에 std::string에 비해서 비용이 아주 적다(문자열을 저장하기 위한 메모리 할당과 해제가 필요 없다!!!).  
  
boost::string_ref는 std::string와 비슷한 멤버를 대부분 가지고 있으나 c_str 멤버는 없다.  
boost::string_ref는 오리지널 문자열을 변경할 수 없고, 메모리 할당도 하지 않는다.  
