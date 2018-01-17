// 버퍼 접근
// 출처: https://sites.google.com/site/boostjp/tips/circular_buffer 

#include <iostream>
#include <boost/circular_buffer.hpp>
    
void disp( char x ) 
{ 
	std::cout << x << ' '; 
}

int main()
{
    boost::circular_buffer<char> c_buf( 5 );

    c_buf.push_front( 'A' );
    c_buf.push_front( 'B' );
    c_buf.push_front( 'C' );
                                                        //  ___ ___ ___ ___ ___
    std::for_each( c_buf.begin(), c_buf.end(), disp );  // |_C_|_B_|_A_|___|___|
    std::cout << std::endl;

    
	boost::circular_buffer<char>::iterator it = c_buf.begin();

    std::cout << c_buf.front()  << std::endl;           // 'C'
    std::cout << *it            << std::endl;           // 'C'

    std::cout << c_buf.back()   << std::endl;           // 'A'
    std::cout << *( c_buf.end() - 1 ) << std::endl;     // 'A'
//  std::cout << *c_buf.end()   << std::endl;           // 이것은 불가능
    std::cout << std::endl;

    c_buf.push_front( 'D' );
    c_buf.push_front( 'E' );
    c_buf.push_front( 'F' );
                                                        //  ___ ___ ___ ___ ___
    std::for_each( c_buf.begin(), c_buf.end(), disp );  // |_F_|_E_|_D_|_C_|_B_|
    std::cout << std::endl;

    std::cout << *it << std::endl;                      // 'C' 
    std::cout << std::endl;

    c_buf.push_front( 'G' );
    c_buf.push_front( 'H' );
                                                        //  ___ ___ ___ ___ ___
    std::for_each( c_buf.begin(), c_buf.end(), disp );  // |_H_|_G_|_F_|_E_|_D_|
    std::cout << std::endl;

//  std::cout << *it << std::endl;                      // 'C'는 사라졌으므로 이것은 불가능

    // -----------------------------
    // 임의 접근
    // -----------------------------
    std::cout << c_buf[0]     << std::endl;             // 'H'
    std::cout << c_buf.at(2)  << std::endl;             // c_buf[2]와 같은 'F' 
    std::cout << std::endl;

    return 0;
}