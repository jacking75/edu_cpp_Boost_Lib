# 개요
- 함수를 나올 때 호출될 코드를 정의하여 자동으로 호출 되도록 한다
- (일본어) https://boostjp.github.io/tips/scope_guard.html
- (일본어) C++11 람다 대응 http://d.hatena.ne.jp/faith_and_brave/20111125/1322199381
- (일본어) this 캡쳐하기 http://d.hatena.ne.jp/faith_and_brave/20120703/1341298054


# 스코프 내의 모든 변수를 캡쳐하기(C++11 전용)
BOOST_SCOPE_EXIT_ALL을 사용한다. 클래스의 멤버 변수도 사용할 수 있음   
```
CPacketSend kPacketSend( SAD_REP_CHECK_ACCOUNT );
BYTE nErrorCode = E_SAI_REP_CHECK_ACCOUNT::FLAG_CHECK_OK;

BOOST_SCOPE_EXIT_ALL(&)
{
    if( nErrorCode != E_SAI_REP_CHECK_ACCOUNT::FLAG_CHECK_OK )
    {
    kPacketSend << nErrorCode;
    kPacketSend << nClientSocketIndexInLoginServer;
    kPacketSend << dwLoginUniqueSocketID;
    kPacketSend << iLoginServerNumber;
    GET_NETWORK_INTERFACE()->SendToServer( iServer, kPacketSend);
    }
};
```
  
  
# BOOST_SCOPE_EXIT를 사용할 때 조심할 점
이것을 사용하는 스코프 내에서 BOOST_SCOPE_EXIT를 정의하기 전에 return 문을 사용하면 안된다.  
```
void Check()
{
   ...........
   if( ... )
   {
     return;
   }

   // 아래가 호출되지 않는다
   BOOST_SCOPE_EXIT_ALL(&)
   {
   };
}
```  

올바르게 사용하려면 아래와 같이 바꾸어야 한다.  
```
void Check()
{
   ...........
   // 아래가 호출되지 않는다
   BOOST_SCOPE_EXIT_ALL(&)
   {
   };

   ....
   if( ... )
   {
     return;
   }


}
```  
  
# BOOST_SCOPE_EXIT
`BOOST_SCOPE_EXIT_END` 매크로를 쌍으로 사용해야 한다  
```
BOOST_SCOPE_EXIT((&x)(&y)) {
    x = x + 1;
    y = y + 1;
} BOOST_SCOPE_EXIT_END
```    