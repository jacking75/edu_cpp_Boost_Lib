### 개요
- Boost 라이브러리 1.53에서 추가된 라이브러리로 문자열의 소유권은 없으며 다만 선두 포인터와 크기만을 가진 문자열 클래스 이다.

이 라이브러리를 사용하면 좋은 예는 아래와 같다.

char szName[16];
void CheckName(const std::string& strName );
....
CheckName( szName );

위의 코드에서는 CheckName에 chat 타입의 문자열을 넘기면 임시적으로 string 오브젝트를 생성한다. CheckName 함수를 호출할 때 마다 불필요한 비용이 발생한다.
만약 아주 빈번하게 CheckName가 호출된다면 임시 오브젝트 생성과 삭제에 대한 비용이 꽤 문제가 될 수 있다.

바로 이런 부분에서 boost::string_ref를 사용하면 좋다.
void CheckName(const boost::string_ref& strName );
로 바꾸면 szName을 담기 위해 저장소를 만들 필요 없이 szName에 대한 포인터와 문자 길이만을 가진 오브젝트를 만들기 때문에 std::string에 비해서 비용이 아주 적다(문자열을 저장하기 위한 메모리 할당과 해제가 필요 없다!!!).

boost::string_ref는 std::string와 비슷한 멤버를 대부분 가지고 있으나 c_str 멤버는 없다.

boost::string_ref는 오리지널 문자열을 변경할 수 없고, 메모리 할당도 하지 않는다.
