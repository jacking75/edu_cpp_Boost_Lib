### C++11 프로그래밍
근래 gcc나 Clang 등이 C++11 지원을 완료함에 따라서 기존에 C++03 방식으로 만들었던 C++ 프로그램에 조금씩 C++11 방식이 적용되고 있다.
얼마전에 Boost 라이브러리가 1.54 버전이 나왔는데 Asio 예제 코드를 03과 11 버전 두 가지로 나누어서 제공하고 있다.
- http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/examples.html
- http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/examples/cpp11_examples.html

이제 03 방식의 관습에서 벗어나서 C++11 방식으로 의식하면서 사용해야 할 것 같다



### C++11의 lambda를 사용한 비동기 완료 함수 등록
- 지금의 C++ 표준은 03 이 아닌 C++11 이다. 새로운 시대에 맞게 Asio 프로그래밍을 할 때 C++11 표준을 사용하는 것이 좋다.
- C++03 표준을 사용하여 Asio를 사용할 때는 Asio의 Accept, Send, Receive 등의 비동기 함수에 완료 함수를 등록할 때 boost::bind를 사용하였지만 C++11 에서는 lambda를 사용한다.
- Asio에서 리모트의 접속을 받아들이는 함수 async_accept를 C++03에서는 아래와 같은 방식으로사용했다.

```
boost::asio::ip::tcp::acceptor m_acceptor;

void handle_accept(Session* pSession, const boost::system::error_code& error)
{
if (!error)
{     
       std::cout << "클라이언트 접속 성공" << std::endl;

       pSession->PostReceive();
}
}

m_acceptor.async_accept( m_pSession->Socket(),
                    boost::bind(&TCP_Server::handle_accept,
                           this,
                           m_pSession,
                           boost::asio::placeholders::error)
                    );
```
<br>  
위의 코드를 C++11 방식으로 바꾸면 아래와 같다.
```
m_acceptor.async_accept( m_pSession->Socket(),
                     [this](boost::system::error_code error)
                    {
                       if (!error)
                       {  
                          std::cout << "클라이언트 접속 성공" << std::endl;
                          m_pSession->PostReceive();
                       }

                       StartAccept();
                    }
                    );
```



### future 사용하기
future를 사용하면 비동기 IO 처리의 결과 값을 얻을 수 있다.
아래의 코드는 future를 사용하여 UDP로 보내기와 받기를 하고 있다. future를 사용하고 있어서 비동기 완료 함수를 지정하지 않아도 된다.
<img src="..\resource\asio_cpp11_future.PNG">

완전한 코드는 'AsynchronousUDPClient_future' 프로젝트에 있다.
[AsynchronousUDPClient_future](..\resource\AsynchronousUDPClient_future.zip)



### std::packaged_task 사용하기
packaged_task를 사용하면 병렬 프로그래밍 패턴의 task 패턴과 같이 Asio에서 백그라운드 작업을 처리할 수 있다.
<img src="..\resource\asio_cpp11_packaged_task_01.png">

<img src="..\resource\asio_cpp11_packaged_task_02.png">

완전한 코드는 'io_service_post_packaged_task' 프로젝트를 참조하기 바란다.
[io_service_post_packaged_task](..\resource\io_service_post_packaged_task.zip)


### C++11. Asio와 코루틴
Boost 라이브러리는 1.53 버전부터 코루틴이 정식으로 포함되었다. Boost 라이브러리 1.54 버전의 Asio에서는 코루틴을 Asio에서도 사용할 수 있게 되었다.
Asio에서 코루틴을 사용하기 위해서는 boost:asio::spawn을 사용한다.

Asio에서 비동기 I/O 함수를 사용할 때는 언제나 Asio의 비동기 함수를 호출하고, 이때 완료 함수를 등록 한다. 그리고 완료 함수가 호출에 의해 작업의 완료를 알게 된다.
<img src="..\resource\asio_spawn_01.png">

boost::asio::spawn을 사용하면 코루틴에 의해 비동기 함수를 호출하면 호출한 곳으로 제어권을 넘긴 후 비동기 작업이 완료하면 비동기 함수 호출 이후 부분에 복귀하여 남은 작업을 처리한다.
아래는 boost::asio::spawn을 사용하여 비동기로 데이터를 보내는 코드이다.
<img src="..\resource\asio_spawn_02.png">


< boost::asio::spawn을 사용하여 비동기로 데이터 보내기 >  
코루틴을 사용해본 경험이 없다면 아마 위의 코드만으로는 잘 이해가 안될 것이다. 그러니 기존의 Chatting 서버를 boost::asio::spawn 사용 버전으로 만든 'ChattingTCPServer_spawn' 프로젝트를 꼭 보고 실행까지 해보기 바란다. boost::asio::spawn을 사용해서 코드가 이전에 비해서 줄어들어서 어렵지 않게 이해할 수 있을 것이다.  
[ChattingTCPServer_spawn.zip](..\resource\ChattingTCPServer_spawn.zip)

- Boost 라이브러리의 asio::spawn 관련 예제 코드
http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/spawn/
