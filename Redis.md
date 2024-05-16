# Boost.Redis
- 1.84 버전에서 생겼다.
- `Asio`를 사용하여 만들었다.
- Boost 라이브러리와 별도로 `OpenSSL` 라이브러리가 필요하다
- 1.84 기준 [문서](https://www.boost.org/doc/libs/1_85_0/libs/redis/doc/html/index.html )
  
Boost.Redis는 Redis 프로토콜 RESP3을 구현한 Boost.Asio 위에 구축된 높은 수준의 Redis 클라이언트 라이브러리이다.  
Boost.Redis를 사용하기 위한 요구 사항은 다음과 같다:  
- Boost. 이 라이브러리는 1.84부터 시작하는 Boost 배포에 포함되어 있다.
- C++ 17 이상.
- Redis 6 이상(RESP3을 지원해야 함).
- Gcc(10, 11, 12), Clang(11, 13, 14) 및 Visual Studio(16 2019, 17 2022).
-  Redis 및 Boost.Asio에 대한 기본 수준의 지식이 있어야 합니다.
  
최신 릴리스는 https://github.com/boostorg/redis/releases 에서 다운로드할 수 있다. 라이브러리 헤더는 include 하위 디렉터리와 소스 컴파일에서 찾을 수 있다.
  
  
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
    
  
## 연결하기 
짧은 수명의 연결을 사용하여 Redis에 핑 명령을 보내는 간단한 애플리케이션부터 시작해 보겠다.  
```
auto co_main(config const& cfg) -> net::awaitable<void>
{
   auto conn = std::make_shared<connection>(co_await net::this_coro::executor);
   conn->async_run(cfg, {}, net::consign(net::detached, conn));
 
   // A request containing only a ping command.
   request req;
   req.push("PING", "Hello world");
 
   // Response where the PONG response will be stored.
   response<std::string> resp;
 
   // Executes the request.
   co_await conn->async_exec(req, resp, net::deferred);
   conn->cancel();
 
   std::cout << "PING: " << std::get<0>(resp).value() << std::endl;
}
```  
`async_run` 및 `async_exec` 함수가 수행하는 역할은 다음과 같다.
- `async_exec`: 요청에 포함된 명령을 실행하고 개별 응답을 해당 객체에 저장한다. 코드의 여러 위치에서 동시에 호출할 수 있다.  
- `async_run`: 확인, 연결, ssl-handshake, resp3-handshake, 상태 확인, 재연결 및 저수준 읽기 및 쓰기 작업 조정(무엇보다도)을 수행한다.
   
  
## Server pushes
- Pubsub
- Keyspace notification
- Client-side caching    
  
연결 클래스는 명령을 실행하는 데 사용되는 것과 동일한 연결에서 호출할 수 있는 `boost::redis::connection::async_receive` 함수를 통해 서버 푸시를 지원한다. 아래 코루틴은 이 함수를 사용하는 방법을 보여준다.    
```
auto
receiver(std::shared_ptr<connection> conn) -> net::awaitable<void>
{
   request req;
   req.push("SUBSCRIBE", "channel");
 
   generic_response resp;
   conn->set_receive_response(resp);
 
   // Loop while reconnection is enabled
   while (conn->will_reconnect()) {
 
      // Reconnect to channels.
      co_await conn->async_exec(req, ignore, net::deferred);
 
      // Loop reading Redis pushes.
      for (;;) {
         error_code ec;
         co_await conn->async_receive(resp, net::redirect_error(net::use_awaitable, ec));
         if (ec)
            break; // Connection lost, break so we can reconnect to channels.
 
         // Use the response resp in some way and then clear it.
         ...
 
         consume_one(resp);
      }
   }
}
```  
    

## Requests
Redis 요청은 하나 이상의 명령으로 구성된다(Redis 설명서에서는 이를 파이프라인이라고 함). 예를 들어
```
// Some example containers.
std::list<std::string> list {...};
std::map<std::string, mystruct> map { ...};
 
// The request can contain multiple commands.
request req;
 
// Command with variable length of arguments.
req.push("SET", "key", "some value", "EX", "2");
 
// Pushes a list.
req.push_range("SUBSCRIBE", list);
 
// Same as above but as an iterator range.
req.push_range("SUBSCRIBE", std::cbegin(list), std::cend(list));
 
// Pushes a map.
req.push_range("HSET", "key", map);
```   
Redis에 요청을 전송하는 것은 이미 설명한 대로 `boost::redis::connection::async_exec`을 사용하여 수행된다.
  
  
### Config flags
요청 내부의 [boost::redis::request::config](https://www.boost.org/doc/libs/1_85_0/libs/redis/doc/html/classboost_1_1redis_1_1request.html#structboost_1_1redis_1_1request_1_1config ) 객체는 몇 가지 중요한 상황에서 [boost::redis::connection](https://www.boost.org/doc/libs/1_85_0/libs/redis/doc/html/classboost_1_1redis_1_1connection.html )이 요청을 처리하는 방법을 지정한다. 이를 주의 깊게 읽어보기 바린다.   
  
    

## Responses
Boost.Redis는 다음 전략을 사용하여 Redis 응답을 지원한다  
- [boost::redis::request](https://www.boost.org/doc/libs/1_85_0/libs/redis/doc/html/classboost_1_1redis_1_1request.html )는 명령의 수가 동적이지 않은 요청에 사용된다.
- 동적: 그렇지 않으면 [boost::redis::generic_response](https://www.boost.org/doc/libs/1_85_0/libs/redis/doc/html/group__high-level-api.html#gaa1dc751c19aca21d7bd661c8b9eea04f )를 사용한다.
  
```
request req;
req.push("PING");
req.push("INCR", "key");
req.push("QUIT");
```  
의 응답에는 세 개의 명령어가 있으며 다음 응답 객체에서 읽을 수 있다.  
```
response<std::string, int, std::string>
```    
응답은 튜플로 작동하며 요청에 명령이 있는 수만큼의 요소를 가져야 한다(아래 예외). 또한 각 튜플 요소가 참조하는 명령에 대한 응답을 저장할 수 있어야 하며, 그렇지 않으면 오류가 발생한다. 요청에서 개별 명령에 대한 응답을 무시하려면 [boost::redis::ignore_t](https://www.boost.org/doc/libs/1_85_0/libs/redis/doc/html/group__high-level-api.html#ga71ed828bcaf291989dcf4050a18601ba ) 태그를 사용한다.
```
// Ignore the second and last responses.
response<std::string, boost::redis::ignore_t, std::string, boost::redis::ignore_t>
```  
  
| Command  | RESP3 type                         | Documentation                      |
|----------|------------------------------------|------------------------------------|
| lpush    | Number                             | https://redis.io/commands/lpush    |
| lrange   | Array                              | https://redis.io/commands/lrange   |
| set      | Simple-string, null or blob-string | https://redis.io/commands/set      |
| get      | Blob-string                        | https://redis.io/commands/get      |
| smembers | Set                                | https://redis.io/commands/smembers |
| hgetall  | Map                                | https://redis.io/commands/hgetall  |  
  
| RESP3 type    | Possible C++ type                              | Type      |
|---------------|------------------------------------------------|-----------|
| Simple-string | std::string                                    | Simple    |
| Simple-error  | std::string                                    | Simple    |
| Blob-string   | std::string, std::vector                       | Simple    |
| Blob-error    | std::string, std::vector                       | Simple    |
| Number        | long long, int, std::size_t, std::string       | Simple    |
| Double        | double, std::string                            | Simple    |
| Null          | std::optional<T>                               | Simple    |
| Array         | std::vector, std::list, std::array, std::deque | Aggregate |
| Map           | std::vector, std::map, std::unordered_map      | Aggregate |
| Set           | std::vector, std::set, std::unordered_set      | Aggregate |
| Push          | std::vector, std::map, std::unordered_map      | Aggregate |  
    
For example, the response to the request  
```
request req;
req.push("HELLO", 3);
req.push_range("RPUSH", "key1", vec);
req.push_range("HSET", "key2", map);
req.push("LRANGE", "key3", 0, -1);
req.push("HGETALL", "key4");
req.push("QUIT");
```   
  
```
response<
   redis::ignore_t,  // hello
   int,              // rpush
   int,              // hset
   std::vector<T>,   // lrange
   std::map<U, V>,   // hgetall
   std::string       // quit
> resp;
```  
  
여기서 둘 다 다른 곳에 표시된 것처럼 async_exec에 전달된다.  
```
co_await conn->async_exec(req, resp, net::deferred);
```  
응답을 완전히 무시하려는 의도는 무시  
```
// Ignores the response
co_await conn->async_exec(req, ignore, net::deferred);
```  
중첩된 집계 또는 이질적인 데이터 유형을 포함하는 응답은 나중에 일반적인 경우에서 특별한 취급을 받게 된다. 이 글을 쓰는 현재 모든 RESP3 유형이 Redis 서버에서 사용되는 것은 아니므로, 실제로는 사용자가 RESP3 사양의 축소된 하위 집합을 사용하게 될 것이다.  
  
  
### Pushes
- "SUBSCRIBE"
- "PSUBSCRIBE"
- "UNSUBSCRIBE"
은 응답 튜플에 포함되지 않아야 한다. 예를 들어, 아래 요청은  
```
request req;
req.push("PING");
req.push("SUBSCRIBE", "channel");
req.push("QUIT");
```    
는 정적 크기 2인 이 튜플 `response<std::string, std::string>` 에서 읽어야 한다.    
  
  
### Null
앱이 Redis 서버에 존재하지 않거나 이미 만료된 키에 액세스하는 경우가 종종 발생하는데, 이러한 경우를 처리하기 위해 Boost.Redis는 `std::optional`을 지원한다. 이를 사용하려면 다음과 같이 `std::optional`로 타입을 감싸면 된다.  
```
response<
   std::optional<A>,
   std::optional<B>,
   ...
   > resp;
 
co_await conn->async_exec(req, resp, net::deferred);
```  
  
  
### Transactions
To read responses to transactions we must first observe that Redis will queue the transaction commands and send their individual responses as elements of an array, the array is itself the response to the EXEC command. For example, to read the response to this request  
```
req.push("MULTI");
req.push("GET", "key1");
req.push("LRANGE", "key2", 0, -1);
req.push("HGETALL", "key3");
req.push("EXEC");
```  
  
use the following response type  
```  
using `boost::redis::ignore;`  

using exec_resp_type = 
   response<
      std::optional<std::string>, // get
      std::optional<std::vector<std::string>>, // lrange
      std::optional<std::map<std::string, std::string>> // hgetall
   >;
 
response<
   boost::redis::ignore_t,  // multi
   boost::redis::ignore_t,  // get
   boost::redis::ignore_t,  // lrange
   boost::redis::ignore_t,  // hgetall
   exec_resp_type,        // exec
> resp;
 
co_await conn->async_exec(req, resp, net::deferred);
```  
  
  
### Serialization
Boost.Redis는 다음과 같은 사용자 정의 포인트를 통해 사용자 정의 유형의 직렬화를 지원한다.  
```
// Serialize.
void boost_redis_to_bulk(std::string& to, mystruct const& obj);
 
// Deserialize
void boost_redis_from_bulk(mystruct& obj, char const* p, std::size_t size, boost::system::error_code& ec)
```  
이러한 함수는 ADL을 통해 액세스되므로 사용자가 글로벌 네임스페이스에서 가져와야 한다. [예제](https://www.boost.org/doc/libs/1_85_0/libs/redis/doc/html/index.html#examples) 섹션에서 json 및 protobuf를 사용하여 직렬화하는 방법을 보여주는 예제를 찾을 수 있다.   
  
    
     
## Examples
- cpp20_intro.cpp: Does not use awaitable operators.
- [cpp20_intro_tls.cpp](https://www.boost.org/doc/libs/1_85_0/libs/redis/doc/html/cpp20__intro__tls_8cpp_source.html ): Communicates over TLS.
- cpp20_containers.cpp: Shows how to send and receive STL containers and how to use transactions.
- cpp20_json.cpp: Shows how to serialize types using Boost.Json.
- [cpp20_protobuf.cpp](https://www.boost.org/doc/libs/1_85_0/libs/redis/doc/html/cpp20__protobuf_8cpp_source.html ): Shows how to serialize types using protobuf.
- [cpp20_resolve_with_sentinel.cpp](https://www.boost.org/doc/libs/1_85_0/libs/redis/doc/html/cpp20__resolve__with__sentinel_8cpp_source.html ): Shows how to resolve a master address using sentinels.
- cpp20_subscriber.cpp: Shows how to implement pubsub with reconnection re-subscription.
- cpp20_echo_server.cpp: A simple TCP echo server.
- cpp20_chat_room.cpp: A command line chat built on Redis pubsub.
- cpp17_intro.cpp: Uses callbacks and requires C++17.
- cpp17_intro_sync.cpp: Runs async_run in a separate thread and performs synchronous calls to

  
### cpp17_intro.cpp
비동기 IO로 Redis에 핑퐁한다  
    
```
#include <boost/redis/connection.hpp>
#include <boost/asio/detached.hpp>
#include <iostream>
 
namespace asio = boost::asio;
using boost::redis::connection;
using boost::redis::request;
using boost::redis::response;
using boost::redis::config;
 
auto main(int argc, char * argv[]) -> int
{
   try {
      config cfg;
 
      if (argc == 3) {
         cfg.addr.host = argv[1];
         cfg.addr.port = argv[2];
      }
 
      request req;
      req.push("PING", "Hello world");
 
      response<std::string> resp;
 
      asio::io_context ioc;
      connection conn{ioc};
 
      conn.async_run(cfg, {}, asio::detached);
 
      conn.async_exec(req, resp, [&](auto ec, auto) {
         if (!ec)
            std::cout << "PING: " << std::get<0>(resp).value() << std::endl;
         conn.cancel();
      });
 
      ioc.run();
 
   } catch (std::exception const& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
   }
}
```        


### cpp17_intro_sync.cpp
동기 IO로 Redis에 핑퐁한다    
  
```
#include "sync_connection.hpp"
 
#include <string>
#include <iostream>
 
using boost::redis::sync_connection;
using boost::redis::request;
using boost::redis::response;
using boost::redis::config;
 
auto main(int argc, char * argv[]) -> int
{
   try {
      config cfg;
 
      if (argc == 3) {
         cfg.addr.host = argv[1];
         cfg.addr.port = argv[2];
      }
 
      sync_connection conn;
      conn.run(cfg);
 
      request req;
      req.push("PING");
 
      response<std::string> resp;
 
      conn.exec(req, resp);
      conn.stop();
 
      std::cout << "Response: " << std::get<0>(resp).value() << std::endl;
 
   } catch (std::exception const& e) {
      std::cerr << e.what() << std::endl;
   }
}
```         
  

### cpp20_intro.cpp
Boost.Redis 사용을 보여주는 핑퐁 예제 코드이다.  
  
```
#include <boost/redis/connection.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/consign.hpp>
#include <iostream>
 
#if defined(BOOST_ASIO_HAS_CO_AWAIT)
 
namespace asio = boost::asio;
using boost::redis::request;
using boost::redis::response;
using boost::redis::config;
using boost::redis::connection;
 
// Called from the main function (see main.cpp)
auto co_main(config cfg) -> asio::awaitable<void>
{
   auto conn = std::make_shared<connection>(co_await asio::this_coro::executor);
   conn->async_run(cfg, {}, asio::consign(asio::detached, conn));
 
   // A request containing only a ping command.
   request req;
   req.push("PING", "Hello world");
 
   // Response where the PONG response will be stored.
   response<std::string> resp;
 
   // Executes the request.
   co_await conn->async_exec(req, resp, asio::deferred);
   conn->cancel();
 
   std::cout << "PING: " << std::get<0>(resp).value() << std::endl;
}
 
#endif // defined(BOOST_ASIO_HAS_CO_AWAIT)  
```  
  
  
### cpp20_containers.cpp
STL 컨테이너를 보내고 받는 방법과 트랜잭션을 사용하는 방법을 보여준다  
    
```
/* Copyright (c) 2018-2022 Marcelo Zimbres Silva (mzimbres@gmail.com)
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE.txt)
 */
 
#include <boost/redis/connection.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/co_spawn.hpp>
#include <map>
#include <vector>
#include <iostream>
 
#if defined(BOOST_ASIO_HAS_CO_AWAIT)
 
namespace asio = boost::asio;
using boost::redis::request;
using boost::redis::response;
using boost::redis::ignore_t;
using boost::redis::ignore;
using boost::redis::config;
using boost::redis::connection;
using boost::asio::awaitable;
using boost::asio::deferred;
using boost::asio::detached;
using boost::asio::consign;
 
void print(std::map<std::string, std::string> const& cont)
{
   for (auto const& e: cont)
      std::cout << e.first << ": " << e.second << "\n";
}
 
void print(std::vector<int> const& cont)
{
   for (auto const& e: cont) std::cout << e << " ";
   std::cout << "\n";
}
 
// Stores the content of some STL containers in Redis.
auto store(std::shared_ptr<connection> conn) -> awaitable<void>
{
   std::vector<int> vec
      {1, 2, 3, 4, 5, 6};
 
   std::map<std::string, std::string> map
      {{"key1", "value1"}, {"key2", "value2"}, {"key3", "value3"}};
 
   request req;
   req.push_range("RPUSH", "rpush-key", vec);
   req.push_range("HSET", "hset-key", map);
 
   co_await conn->async_exec(req, ignore, deferred);
}
 
auto hgetall(std::shared_ptr<connection> conn) -> awaitable<void>
{
   // A request contains multiple commands.
   request req;
   req.push("HGETALL", "hset-key");
 
   // Responses as tuple elements.
   response<std::map<std::string, std::string>> resp;
 
   // Executes the request and reads the response.
   co_await conn->async_exec(req, resp, deferred);
 
   print(std::get<0>(resp).value());
}
 
// Retrieves in a transaction.
auto transaction(std::shared_ptr<connection> conn) -> awaitable<void>
{
   request req;
   req.push("MULTI");
   req.push("LRANGE", "rpush-key", 0, -1); // Retrieves
   req.push("HGETALL", "hset-key"); // Retrieves
   req.push("EXEC");
 
   response<
      ignore_t, // multi
      ignore_t, // lrange
      ignore_t, // hgetall
      response<std::optional<std::vector<int>>, std::optional<std::map<std::string, std::string>>> // exec
   > resp;
 
   co_await conn->async_exec(req, resp, deferred);
 
   print(std::get<0>(std::get<3>(resp).value()).value().value());
   print(std::get<1>(std::get<3>(resp).value()).value().value());
}
 
// Called from the main function (see main.cpp)
awaitable<void> co_main(config cfg)
{
   auto conn = std::make_shared<connection>(co_await asio::this_coro::executor);
   conn->async_run(cfg, {}, consign(detached, conn));
 
   co_await store(conn);
   co_await transaction(conn);
   co_await hgetall(conn);
   conn->cancel();
}
 
#endif // defined(BOOST_ASIO_HAS_CO_AWAIT)
```    
  
  
### cpp20_json.cpp
  
```
#include <boost/redis/connection.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/detached.hpp>
#include <boost/describe.hpp>
#include <boost/asio/consign.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <string>
#include <iostream>
 
#if defined(BOOST_ASIO_HAS_CO_AWAIT)
 
#include <boost/json/serialize.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/value_from.hpp>
#include <boost/json/value_to.hpp>
#include <boost/redis/resp3/serialization.hpp>
 
namespace asio = boost::asio;
using namespace boost::describe;
using boost::redis::request;
using boost::redis::response;
using boost::redis::ignore_t;
using boost::redis::config;
using boost::redis::connection;
 
// Struct that will be stored in Redis using json serialization. 
struct user {
   std::string name;
   std::string age;
   std::string country;
};
 
// The type must be described for serialization to work.
BOOST_DESCRIBE_STRUCT(user, (), (name, age, country))
 
// Boost.Redis customization points (example/json.hpp)
void boost_redis_to_bulk(std::string& to, user const& u)
   { boost::redis::resp3::boost_redis_to_bulk(to, boost::json::serialize(boost::json::value_from(u))); }
 
void boost_redis_from_bulk(user& u, std::string_view sv, boost::system::error_code&)
   { u = boost::json::value_to<user>(boost::json::parse(sv)); }
 
auto co_main(config cfg) -> asio::awaitable<void>
{
   auto ex = co_await asio::this_coro::executor;
   auto conn = std::make_shared<connection>(ex);
   conn->async_run(cfg, {}, asio::consign(asio::detached, conn));
 
   // user object that will be stored in Redis in json format.
   user const u{"Joao", "58", "Brazil"};
 
   // Stores and retrieves in the same request.
   request req;
   req.push("SET", "json-key", u); // Stores in Redis.
   req.push("GET", "json-key"); // Retrieves from Redis.
 
   response<ignore_t, user> resp;
 
   co_await conn->async_exec(req, resp, asio::deferred);
   conn->cancel();
 
   // Prints the first ping
   std::cout
      << "Name: " << std::get<1>(resp).value().name << "\n"
      << "Age: " << std::get<1>(resp).value().age << "\n"
      << "Country: " << std::get<1>(resp).value().country << "\n";
}
 
#endif // defined(BOOST_ASIO_HAS_CO_AWAIT)
```  
  

### cpp20_subscriber.cpp

```
#include <boost/redis/connection.hpp>
#include <boost/redis/logger.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/consign.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>
 
#if defined(BOOST_ASIO_HAS_CO_AWAIT)
 
namespace asio = boost::asio;
using namespace std::chrono_literals;
using boost::redis::request;
using boost::redis::generic_response;
using boost::redis::consume_one;
using boost::redis::logger;
using boost::redis::config;
using boost::redis::ignore;
using boost::redis::error;
using boost::system::error_code;
using boost::redis::connection;
using signal_set = asio::deferred_t::as_default_on_t<asio::signal_set>;
 
/* This example will subscribe and read pushes indefinitely.
 *
 * To test send messages with redis-cli
 *
 *    $ redis-cli -3
 *    127.0.0.1:6379> PUBLISH channel some-message
 *    (integer) 3
 *    127.0.0.1:6379>
 *
 * To test reconnection try, for example, to close all clients currently
 * connected to the Redis instance
 *
 * $ redis-cli
 * > CLIENT kill TYPE pubsub
 */
 
// Receives server pushes.
auto
receiver(std::shared_ptr<connection> conn) -> asio::awaitable<void>
{
   request req;
   req.push("SUBSCRIBE", "channel");
 
   generic_response resp;
   conn->set_receive_response(resp);
 
   // Loop while reconnection is enabled
   while (conn->will_reconnect()) {
 
      // Reconnect to the channels.
      co_await conn->async_exec(req, ignore, asio::deferred);
 
      // Loop reading Redis pushs messages.
      for (error_code ec;;) {
         // First tries to read any buffered pushes.
         conn->receive(ec);
         if (ec == error::sync_receive_push_failed) {
            ec = {};
            co_await conn->async_receive(asio::redirect_error(asio::use_awaitable, ec));
         }
 
         if (ec)
            break; // Connection lost, break so we can reconnect to channels.
 
         std::cout
            << resp.value().at(1).value
            << " " << resp.value().at(2).value
            << " " << resp.value().at(3).value
            << std::endl;
 
         consume_one(resp);
      }
   }
}
 
auto co_main(config cfg) -> asio::awaitable<void>
{
   auto ex = co_await asio::this_coro::executor;
   auto conn = std::make_shared<connection>(ex);
   asio::co_spawn(ex, receiver(conn), asio::detached);
   conn->async_run(cfg, {}, asio::consign(asio::detached, conn));
 
   signal_set sig_set(ex, SIGINT, SIGTERM);
   co_await sig_set.async_wait();
 
   conn->cancel();
}
 
#endif // defined(BOOST_ASIO_HAS_CO_AWAIT)
```  
      
  
### cpp20_echo_server.cpp  
  
```
#include <boost/redis/connection.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/co_spawn.hpp>
#include <iostream>
 
#if defined(BOOST_ASIO_HAS_CO_AWAIT)
 
namespace asio = boost::asio;
using tcp_socket = asio::deferred_t::as_default_on_t<asio::ip::tcp::socket>;
using tcp_acceptor = asio::deferred_t::as_default_on_t<asio::ip::tcp::acceptor>;
using signal_set = asio::deferred_t::as_default_on_t<asio::signal_set>;
using boost::redis::request;
using boost::redis::response;
using boost::redis::config;
using boost::system::error_code;
using boost::redis::connection;
using namespace std::chrono_literals;
 
auto echo_server_session(tcp_socket socket, std::shared_ptr<connection> conn) -> asio::awaitable<void>
{
   request req;
   response<std::string> resp;
 
   for (std::string buffer;;) {
      auto n = co_await asio::async_read_until(socket, asio::dynamic_buffer(buffer, 1024), "\n");
      req.push("PING", buffer);
      co_await conn->async_exec(req, resp, asio::deferred);
      co_await asio::async_write(socket, asio::buffer(std::get<0>(resp).value()));
      std::get<0>(resp).value().clear();
      req.clear();
      buffer.erase(0, n);
   }
}
 
// Listens for tcp connections.
auto listener(std::shared_ptr<connection> conn) -> asio::awaitable<void>
{
   try {
      auto ex = co_await asio::this_coro::executor;
      tcp_acceptor acc(ex, {asio::ip::tcp::v4(), 55555});
      for (;;)
         asio::co_spawn(ex, echo_server_session(co_await acc.async_accept(), conn), asio::detached);
   } catch (std::exception const& e) {
      std::clog << "Listener: " << e.what() << std::endl;
   }
}
 
// Called from the main function (see main.cpp)
auto co_main(config cfg) -> asio::awaitable<void>
{
   auto ex = co_await asio::this_coro::executor;
   auto conn = std::make_shared<connection>(ex);
   asio::co_spawn(ex, listener(conn), asio::detached);
   conn->async_run(cfg, {}, asio::consign(asio::detached, conn));
 
   signal_set sig_set(ex, SIGINT, SIGTERM);
   co_await sig_set.async_wait();
   conn->cancel();
}
 
#endif // defined(BOOST_ASIO_HAS_CO_AWAIT)
``` 
  
  
### cpp20_chat_room.cpp
  
```
#include <boost/redis/connection.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <unistd.h>
#include <iostream>
 
#if defined(BOOST_ASIO_HAS_CO_AWAIT)
#if defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)
 
namespace asio = boost::asio;
using stream_descriptor = asio::deferred_t::as_default_on_t<asio::posix::stream_descriptor>;
using signal_set = asio::deferred_t::as_default_on_t<asio::signal_set>;
using boost::asio::async_read_until;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::consign;
using boost::asio::deferred;
using boost::asio::detached;
using boost::asio::dynamic_buffer;
using boost::asio::redirect_error;
using boost::asio::use_awaitable;
using boost::redis::config;
using boost::redis::connection;
using boost::redis::generic_response;
using boost::redis::ignore;
using boost::redis::request;
using boost::system::error_code;
using namespace std::chrono_literals;
 
// Chat over Redis pubsub. To test, run this program from multiple
// terminals and type messages to stdin.
 
auto
receiver(std::shared_ptr<connection> conn) -> awaitable<void>
{
   request req;
   req.push("SUBSCRIBE", "channel");
 
   generic_response resp;
   conn->set_receive_response(resp);
 
   while (conn->will_reconnect()) {
 
      // Subscribe to channels.
      co_await conn->async_exec(req, ignore, deferred);
 
      // Loop reading Redis push messages.
      for (error_code ec;;) {
         co_await conn->async_receive(redirect_error(use_awaitable, ec));
         if (ec)
            break; // Connection lost, break so we can reconnect to channels.
         std::cout
            << resp.value().at(1).value
            << " " << resp.value().at(2).value
            << " " << resp.value().at(3).value
            << std::endl;
         resp.value().clear();
      }
   }
}
 
// Publishes stdin messages to a Redis channel.
auto publisher(std::shared_ptr<stream_descriptor> in, std::shared_ptr<connection> conn) -> awaitable<void>
{
   for (std::string msg;;) {
      auto n = co_await async_read_until(*in, dynamic_buffer(msg, 1024), "\n");
      request req;
      req.push("PUBLISH", "channel", msg);
      co_await conn->async_exec(req, ignore, deferred);
      msg.erase(0, n);
   }
}
 
// Called from the main function (see main.cpp)
auto co_main(config cfg) -> awaitable<void>
{
   auto ex = co_await asio::this_coro::executor;
   auto conn = std::make_shared<connection>(ex);
   auto stream = std::make_shared<stream_descriptor>(ex, ::dup(STDIN_FILENO));
 
   co_spawn(ex, receiver(conn), detached);
   co_spawn(ex, publisher(stream, conn), detached);
   conn->async_run(cfg, {}, consign(detached, conn));
 
   signal_set sig_set{ex, SIGINT, SIGTERM};
   co_await sig_set.async_wait();
   conn->cancel();
   stream->cancel();
}
 
#else // defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)
auto co_main(config const&) -> awaitable<void>
{
   std::cout << "Requires support for posix streams." << std::endl;
   co_return;
}
#endif // defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)
#endif // defined(BOOST_ASIO_HAS_CO_AWAIT)
```  



