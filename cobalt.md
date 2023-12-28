# Boost.Cobalt
- 1.84 버전에서 새로 생긴 라이브러리  
- [1.84 기준 공식 문서](https://www.boost.org/doc/libs/1_84_0/libs/cobalt/doc/html/index.html )  
- 아래 글은 공식 문서의 글을 중심으로 정리(혹은 번역)한 것이다
  
  
## 동기 
node.js 및 python과 같은 많은 프로그래밍 언어는 사용하기 쉬운 단일 스레드 동시성 프레임워크를 제공한다. 동기식 코드보다 복잡하지만 단일 스레드 비동시성은 멀티 스레딩의 많은 함정과 오버헤드를 피할 수 있다.  
  
즉, 하나의 코루틴이 작동하는 동안 다른 코루틴은 이벤트(예: 서버의 응답)를 기다릴 수 있다. 따라서 단일 스레드에서 여러 작업을 동시에 수행하는 애플리케이션을 작성할 수 있다.  
  
이 라이브러리는 C++에 이를 제공하기 위한 것으로, node.js와 유사한 간단한 단일 스레드 비동기성 및 `boost.beast`, `boost.mysql` 또는 `boost.redis`와 같은 기존 라이브러리와 함께 작동하는 파이썬의 asyncio를 제공합니다. `boost.asio`를 기반으로 한다.  
  
다른 언어의 개념 모음을 가져와 C++20 코루틴을 기반으로 제공한다.   
- 비동기 main 및 threads와 같은 쉬운 비동기 기본 함수를 제공한다.
- promise 및 제너레이터 타입들
- 연산 래퍼
- 비동기 scope
- race
- channel
 
`asio::awaitable` 및 `asio::experimental::coro` 와 달리 코발트 코루틴은 개방적이다. 즉, `asio::awaitable` 은 다른 `asio::awaitable`에 의해서만 대기하고 대기할 수 있으며 코루틴에 특정한 동기화 메커니즘을 제공하지 않는다.  
반면에 코발트는 코루틴 및 어웨이터블과 함께 작동하도록 최적화된 코루틴 전용 채널과 다양한 대기 유형(race, gather 등)을 제공한다.  
  
  
## Async programming
`http_get`의 동기 버전  
```
std::string http_get(std:string_view url);

int main(int argc, char * argv[])
{
    auto res = http_get("https://boost.org");
    printf("%s", res.c_str());
    return 0;
}
```
  
`http_get`의 비동기 버전  
```
std::string http_get(std:string_view url);

int main(int argc, char * argv[])
{
    std::string other_res;

    std::thread thr{[&]{ other_res = http_get("https://cppalliance.org"); }};
    auto res = http_get("https://boost.org");
    thr.join();

    printf("%s", res.c_str());
    printf("%s", other_res.c_str());
    return 0;
}
```  
이 방법은 작동하지만 프로그램은 대부분의 시간을 입력을 기다리는 데 소비하게 된다. 운영 체제는 IO를 비동기적으로 수행할 수 있는 API를 제공하며, boost.asio와 같은 라이브러리는 비동기 작업을 관리할 수 있는 이식 가능한 방법을 제공한다. Asio 자체는 완료를 처리하는 방법을 지정하지 않는다. 이 라이브러리(boost.cobalt)는 코루틴/어웨이터블을 통해 이 모든 것을 관리할 수 있는 방법을 제공한다.   
  
```
cobalt::promise<std::string> http_cobalt_get(std:string_view url);

cobalt::main co_main(int argc, char * argv[])
{
    auto [res, other_res] =
            cobalt::join(
                http_cobalt_get(("https://boost.org"),
                http_cobalt_get(("https://cppalliance.org")
            );

    printf("%s", res.c_str());
    printf("%s", other_res.c_str());
    return 0;
}
```   
위 코드에서 요청을 수행하는 비동기 함수는 운영 체제 API를 활용하여 실제 IO가 차단되지 않도록 한다. 즉, 두 함수가 완료되기를 기다리는 동안 작업은 인터리빙되고 차단되지 않는다. 동시에 cobalt는 콜백 지옥에서 벗어날 수 있는 코루틴 프리미티브를 제공한다.  
  
  
## Tutorial
 
### delay  
example/delay.cpp  
```
cobalt::main co_main(int argc, char * argv[])  (1)
{
  asio::steady_timer tim{co_await asio::this_coro::executor,   (2)
                         std::chrono::milliseconds(std::stoi(argv[1]))}; (3)
  co_await tim.async_wait(cobalt::use_op); (4)
  co_return 0;  (5)
}  
```   
(1) `co_main`` 함수는 사용 시 암시적 메인을 정의하며, 비동기 코드를 실행하기 위한 환경을 설정하는 가장 쉬운 방법이다.  
(2) 현재 코루틴 프로미스에서 실행자를 가져온다.  
(3) 인수를 사용하여 타임아웃을 설정한다.  
(4) cobalt::use_op를 사용하여 대기를 수행한다.  
(5) 암시적 메인에서 반환되는 값을 반환한다.     
    
    
이 예제에서는 위와 같이 `co_main`이 정의된 경우 메인 코루틴을 제공하는 `cobalt/main.hpp` 헤더를 사용한다. 여기에는 몇 가지 장점이 있다:
- 환경이 올바르게 설정된다(실행기 및 메모리).
- 컨텍스트가 싱글 스레드라는 신호를 asio에 보낸다.
- SIGINT & SIGTERM이 포함된 asio::signal_set이 자동으로 취소에 연결된다(즉, Ctrl+C가 취소를 유발한다).
  
그런 다음 이 코루틴은 프로미스(코루틴 상태의 C++ 이름인 프로미스)에 실행자를 갖는다. `cobalt/promise.hpp`와 혼동하지 말자.) 이 `coro`  네임스페이스의 `dummy-awaitables`을 통해 얻을 수 있다.  
  
그런 다음 타이머를 구성하고 `use_op`로 `async_wait`를 시작할 수 있다. 코발트는 `asio`와 상호 작용하기 위해 `co_await`하는 여러 가지 방법을 제공하며, 그 중 `use_op`가 가장 쉽다.  
  
  
### echo server
여기서는 모든 곳에서 `use_op`(asio 완료) 토큰을 사용할 것이므로 마지막 매개변수를 건너뛸 수 있도록 기본 완료 토큰을 사용하겠다.  
  
example/echo_server.cpp declarations  
```
namespace cobalt = boost::cobalt;
using boost::asio::ip::tcp;
using boost::asio::detached;
using tcp_acceptor = cobalt::use_op_t::as_default_on_t<tcp::acceptor>;
using tcp_socket   = cobalt::use_op_t::as_default_on_t<tcp::socket>;
namespace this_coro = boost::cobalt::this_coro;  
```  
  
에코 함수를 프로미스 코루틴으로 작성하고 있다. 이 코루틴은 열성적인 코루틴으로 기본값으로 권장되며, 지연 코루틴이 필요한 경우 task를 사용할 수 있다.  
example/echo_server.cpp echo function  
```
cobalt::promise<void> echo(tcp_socket socket)
{
  try   (1)
  {
    char data[4096];
    while (socket.is_open()) (2)
    {
      std::size_t n = co_await socket.async_read_some(boost::asio::buffer(data));  (3)
      co_await async_write(socket, boost::asio::buffer(data, n));   (4)
    }
  }
  catch (std::exception& e)
  {
    std::printf("echo: exception: %s\n", e.what());
  }
}
```    
(1) `use_op`` 완료 토큰을 사용할 때 I/O 오류는 C++ 예외로 변환된다. 또한 코루틴이 취소되는 경우(예: 사용자가 Ctrl-C를 눌러서) 예외도 발생한다. 이러한 조건에서는 오류를 출력하고 루프를 종료한다.
(2) 취소(예외)가 발생하거나 사용자가 연결을 닫을 때까지 루프를 실행한다.  
(3) 사용 가능한 만큼 읽는다.
(4) 읽은 바이트를 모두 쓴다.    
  
echo를 호출하면 `async_read_some`까지 코드를 즉시 실행한 다음 호출자에게 제어권을 반환한다.  
다음으로 acceptor 함수도 필요하다. 여기서는 제너레이터를 사용하여 acceptor 상태를 관리한다. 이 코루틴은 `co_return`` 표현식에 도달할 때까지 여러 번 `co_awaited` 할 수 있는 코루틴이다.
   
example/echo_server.cpp listen function  
```
cobalt::generator<tcp_socket> listen()
{
  tcp_acceptor acceptor({co_await cobalt::this_coro::executor}, {tcp::v4(), 55555});
  for (;;)   (1)
  {
    tcp_socket sock = co_await acceptor.async_accept();   (2)
    co_yield std::move(sock);  (3)
  }
  co_return tcp_socket{acceptor.get_executor()};  (4)
}
```  
(1) 취소하면 여기서도 co_await에서 예외가 발생하게 된다.
(2) 비동기적으로 연결을 수락한다.
(3) 대기 중인 코루틴에 반환한다.
(4) co_return은 C++ 준수를 위한 값을 반환한다.
  
wait_group은 실행 중인 에코 함수를 관리하는 데 사용된다. 이 클래스는 실행 중인 에코 코루틴을 취소하고 대기한다.  
리스너는 소멸되면 저절로 멈추기 때문에 리스너에 대해서도 동일한 작업을 수행할 필요가 없다. 제너레이터의 소멸자는 이를 취소한다.  
promise 호출하는 것만으로도 실행할 수 있다. 그런 다음 이러한 promise를 wait_group에 넣어 범위 종료 시 모든 워커를 해체할 수 있도록 한다.  
  
example/echo_server.cpp co_main function  
```
cobalt::main co_main(int argc, char ** argv)
{
  co_await cobalt::with(cobalt::wait_group(), &run_server);  (1)
  co_return 0u;
}
```  
(1)	비동기 범위로 run_server를 실행한다.  
  
위에 표시된 with 함수는 wait_group과 같은 리소스를 사용하여 함수를 실행한다. 범위에서 with를 종료하면 비동기 해체 함수인 `& co_await`가 호출된다. 이렇게 하면 co_main이 존재하기 전에 모든 연결이 제대로 종료된다.
    
### Advanced examples
https://www.boost.org/doc/libs/1_84_0/libs/cobalt/doc/html/index.html#advanced_examples    
  
  
## Threading
이 라이브러리는 설계상 **싱글 스레드**로 되어 있는데, 이는 재시작을 간소화하여 레이스와 같은 동기화 처리를 더 효율적으로 처리하기 때문이다. 레이스는 데이터 손실을 방지하기 위해 모든 레이스된 대기열을 잠가야 하는데, 이는 요소를 추가할 때마다 차단해야 하고 더 악화될 수 있다.  
>>> task(예: 스폰 사용)를 제외하고는 코루틴이 생성된 스레드가 아닌 다른 스레드에서 재개되도록 할 수 없다.   
  
기술적 이유로는 코루틴을 전환하는 가장 효율적인 방법은 아래와 같이 `await_suspend`에서 새 코루틴의 핸들을 반환하는 것이기 때문이다:  
```
struct my_awaitable
{
    bool await_ready();
    std::coroutine_handle<T> await_suspend(std::coroutine_handle<U>);
    void await_resume();
};
```  
   
이 경우 대기 중인 코루틴은 `await_suspend`가 호출되기 전에 일시 중단되고 반환된 코루틴이 다시 시작된다. 물론 실행자를 거쳐야 하는 경우에는 작동하지 않는다.  
  
이는 대기 중인 코루틴뿐만 아니라 채널에도 적용된다. 이 라이브러리의 채널은 침입형 어웨이터블 목록을 사용하며, 쓰기 작업의 await_suspend에서 읽기(따라서 일시 중단된) 코루틴의 핸들을 반환할 수 있다.      
   
## TODO: 동일한 스레드인지 스레드 번호 찍어 보기
```
cobalt::task<void> delay(std::chrono::milliseconds ms)
{
  asio::steady_timer tim{co_await cobalt::this_coro::executor, ms};
  co_await tim.async_wait(cobalt::use_op);
}

cobalt::main co_main(int argc, char *argv[])
{
  co_await delay(std::chrono::milliseconds(50));
  co_return 0;
}
``` 

## spawn
스폰 함수를 사용하면 `asio executor`/`execution_context` 에서 작업을 실행하고 완료 토큰으로 결과를 소비할 수 있다.
Spawn은 시작을 디스패치하고 완료를 게시합니다. 따라서 task를 사용하여 다른 실행기에서 작업을 실행하고 use_op를 사용하여 현재 실행기에서 결과를 소비하는 것이 안전합니다. 즉, 스폰을 사용하여 스레드를 교차할 수 있습니다.
```
cobalt::task<int> work();

int main(int argc, char *argv[])
{
  asio::io_context ctx{BOOST_ASIO_CONCURRENCY_HINT_1};
  auto f = spawn(ctx, work(), asio::use_future);
  ctx.run();

  return f.get();
}
```  
  

## run
run 함수는 spawn과 유사하지만 동기적으로 실행된. 내부적으로 실행 컨텍스트와 메모리 리소스를 설정한다.  
이는 코발트 코드를 동기식 애플리케이션에 통합할 때 유용할 수 있다.    
```
cobalt::task<int> work();

int main(int argc, char *argv[])
{
  return run(work());
}
```  
  
  
## thread
thread 타입은 메인과 유사하지만 signal_set을 사용하지 않는 환경을 만드는 또 다른 방법이다.    
```
cobalt::thread my_thread()
{
  auto exec = co_await cobalt::this_coro::executor;             
  asio::steady_timer tim{exec, std::chrono::milliseconds(50)}; 
  co_await tim.async_wait(cobalt::use_op);                      
  co_return 0;
}

int main(int argc, char * argv[])
{
  auto thr = my_thread();
  thr.join();
  return 0;
}
```
