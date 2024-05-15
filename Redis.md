# Boost.Redis
- 1.84 버전에서 생겼다.
- `Asio`를 사용하여 만들었다.
- Boost 라이브러리와 별도로 `OpenSSL` 라이브러리가 필요하다
- 1.84 기준 [문서](https://www.boost.org/doc/libs/1_84_0/libs/redis/doc/html/index.html )

## 사용 준비
`Redis 6.0` 이상 버전만 사용할 수 있다.  
Redis 라이브러리를 사용하려면 `OpenSSL` 라이브러리를 설치해야한다.  
Redis 라이브러리쪽에서 무조건 OpenSSL을 사용하고 있다.  
 
### Windows
`vcpkg`로 OpenSSL을 설칳라는 것이 가장 간단하다.  
vcpkg에 대해서 잘 모른다면 구글링으로 알아보기 바란다. 한글로된 문서도 꽤 많다. 
  
동적 라이브러리로 설치하기  
```
vcpkg install openssl:x64-windows
```  
동적 라이브러리로 설치하기
```
vcpkg install boost-windows:x64-windows-static
```   
  
정적 라이브러리 설치로 사용하려고 했지만 링크 에러로 빌드가 되지 않았다.   
**동적 라이브러리로 설치하기를 추천한다**   
  
만약 직접 OpenSSl을 빌드하고 싶다면 이 글을 참고하기 바란다.  
[(일어) Windows 에서 OpenSSL 빌드하기](https://qiita.com/etaka/items/7a0784325c1f8c01ae41 )
  
