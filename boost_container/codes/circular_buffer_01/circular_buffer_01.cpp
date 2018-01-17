// 개념
//출처: https://sites.google.com/site/boostjp/tips/circular_buffer 

#include <iostream>
#include <boost/circular_buffer.hpp>

void disp( char x ) 
{ 
	std::cout << x << ' '; 
}

int main()
{
    boost::circular_buffer<char> c_buf(3);

    c_buf.push_front( 'a' );
    std::cout << c_buf[0] << std::endl;                  // 임의 접근 가능
    std::for_each( c_buf.begin(), c_buf.end(), disp );   // 반복자 사용 가능
    std::cout << std::endl;

    c_buf.push_front( 'b' );
    std::cout << c_buf[0] << " " << c_buf[1] << std::endl;

    c_buf.push_front( 'c' );
    std::cout << c_buf[0] << " " << c_buf[1] << " " << c_buf[2] << std::endl;

    c_buf.push_front( 'd' );
    std::cout << c_buf[0] << " " << c_buf[1] << " " << c_buf[2] << std::endl;

    return 0;
}