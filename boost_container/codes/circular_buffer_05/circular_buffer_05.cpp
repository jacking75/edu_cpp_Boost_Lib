// 버퍼의 사용 현황 확인
// 출처: https://sites.google.com/site/boostjp/tips/circular_buffer


#include <iostream>
#include <boost/circular_buffer.hpp>

void disp( char x ) 
{ 
	std::cout << x << ' '; 
}

int main()
{
    boost::circular_buffer<char> c_buf( 3 );

    c_buf.set_capacity( 7 );                            // 버퍼의 허용 사이즈를 '7'로 확장

    std::cout << c_buf.size() << std::endl;             // 현재는 비어 있으므로 사이즈는 '0'
    std::cout << c_buf.capacity() << std::endl;         // 버퍼의 허용 사이즈는 '7'
    if( c_buf.empty() ){                                // empty()==true
        std::cout << "buffer empty." << std::endl;
    }

    c_buf.push_front( 'a' );
    std::cout << c_buf.size() << std::endl;             // 1개 넣어서 사이즈는 '1'

    c_buf.set_capacity( 5 );                            // 허용 사이즈를 '5'로 작게 만든다

    // 어느 정도 더 넣을 수 있는지 조사
    std::cout << c_buf.reserve()  << std::endl;         // capacity()-size() 의미

    c_buf.push_front( 'b' );
    c_buf.push_front( 'c' );
    c_buf.push_front( 'd' );
    c_buf.push_front( 'e' );

    if( c_buf.full() ){                               // full()==true로 버퍼가 꽉 찼다
        std::cout << "buffer full." << std::endl;
    }
                                                        //  ___ ___ ___ ___ ___
    std::for_each( c_buf.begin(), c_buf.end(), disp );  // |_e_|_d_|_c_|_b_|_a_|
    std::cout << std::endl;
    std::cout << c_buf.size() << std::endl;             // 사이즈는 '5'


    c_buf.set_capacity( 3 );                            // 버퍼의 값이 차 있어도 '3'으로 만들 수 있다
                                                        //  ___ ___ ___ 
    std::for_each( c_buf.begin(), c_buf.end(), disp );  // |_e_|_d_|_c_|
    std::cout << std::endl;
    std::cout << c_buf.size() << std::endl;             // 사이즈는 '3'

    return 0;
}