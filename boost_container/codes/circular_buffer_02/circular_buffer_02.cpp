// 추가와 삭제
// 출처: https://sites.google.com/site/boostjp/tips/circular_buffer 


#include <iostream>
#include <boost/circular_buffer.hpp>

void disp( char x ) 
{ 
	std::cout << x << ' '; 
}

int main()
{
	{
		boost::circular_buffer<char> c_buf(3);
		c_buf.push_back( 'x' );
		c_buf.push_back( 'y' );
		c_buf.push_back( 'z' );

		std::for_each( c_buf.begin(), c_buf.end(), disp );
		std::cout << std::endl;


		c_buf.push_back( '1' );

		std::for_each( c_buf.begin(), c_buf.end(), disp );
	}

	std::cout << std::endl;
	std::cout << std::endl;

	{
		boost::circular_buffer<char> c_buf(3);

		c_buf.push_back( 'x' );
		c_buf.push_back( 'y' );
		c_buf.push_back( 'z' );

		std::for_each( c_buf.begin(), c_buf.end(), disp );
		std::cout << std::endl;

		c_buf.pop_back(); // 끝에서 'z' 삭제
		std::for_each( c_buf.begin(), c_buf.end(), disp );
		std::cout << std::endl;

		c_buf.pop_front(); // 앞에서 'x' 삭제
		std::for_each( c_buf.begin(), c_buf.end(), disp );
	}

    return 0;
}