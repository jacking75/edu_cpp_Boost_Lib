### 자기 자신의 IP 얻기
자기 자신(컴퓨터)의 IP 어드레스는 다음과 같은 방법으로 얻을 수 있다.
처음에는 IPv6 주소, 두 번째는 IPv4 주소가 나온다.

```
std::cout << "1" << std::endl;
boost::asio::io_service io_service;
boost::asio::ip::tcp::resolver resolver(io_service);
boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
boost::asio::ip::tcp::resolver::iterator end; // End marker.
while (iter != end)
{
	boost::asio::ip::tcp::endpoint ep = *iter++;
	std::cout << ep << std::endl;
}

{
	std::cout << "2" << std::endl;
	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(),"");
	boost::asio::ip::tcp::resolver::iterator it=resolver.resolve(query);

	while(it!=boost::asio::ip::tcp::resolver::iterator())
	{
		boost::asio::ip::address addr=(it++)->endpoint().address();
		if(addr.is_v6())
		{
			std::cout<<"ipv6 address: ";
		}
		else
			std::cout<<"ipv4 address: ";

		std::cout<<addr.to_string()<<std::endl;

	}
}
```



### windows::object_handle 다루기
Boost.Asio의 장점 중의 하나가 멀티 플랫폼을 지원하면서 각 플랫폼의 특징을 죽이지 않고 사용할 수 있도록 해주는 것이다.  
Boost 라이브러리 1.49.0 버전에서 Windows에서 HANDLE의 비동기 조작을 할 수 있도록 해주는 windows::object_handle 클래스가 추가 되었다.  
아래의 코드는 프로세스를 비동기로 실행 후 종료할 때까지 대기 하다가 프로세스가 종료하면 등록된 함수를 호출한다.  

```
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/windows/object_handle.hpp>

namespace asio = boost::asio;

void on_end_process(const boost::system::error_code& ec)
{
    if (!ec)
       {
        std::cout << "end process" << std::endl;
    }
}

int main()
{
    asio::io_service io_service;

    PROCESS_INFORMATION pi = {};
    STARTUPINFO si = {};

    si.cb = sizeof(si);

    std::string proc = "notepad";
       CreateProcessA(NULL, &proc[0], NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, (LPSTARTUPINFOA)&si, &pi);

    asio::windows::object_handle process(io_service, pi.hProcess);
    process.async_wait(on_end_process);

    io_service.run();
}
```

object_handle에는 wait/async_wait()가 있으며 내부에서 WaitForSingleObjecr를 호출한다. 프로세스 이외에도 CreateEvent로 만든 핸들을 SetEvent 될 때까지 대기시킬 때도 사용할 수 있다.

출처: http://d.hatena.ne.jp/faith_and_brave/20120229/1330496131
