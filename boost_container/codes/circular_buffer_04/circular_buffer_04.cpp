// 실제 메모리 주소
// 출처: https://sites.google.com/site/boostjp/tips/circular_buffer 

#include <iostream>
#include <boost/circular_buffer.hpp>
#include <boost/lambda/lambda.hpp>

// 메모리 표시
void mem_dump( char *p, int num )
{
	std::for_each( p, p+num, [] (char data) { std::cout << data << ' '; });
    std::cout << std::endl;
}

int main()
{
    boost::circular_buffer<char> c_buf( 5 );
    c_buf.push_front( 'A' );
    c_buf.push_front( 'B' );
    c_buf.push_front( 'C' );
    c_buf.push_front( 'D' );
    c_buf.push_front( 'E' );
    c_buf.push_front( 'F' );    // 이 이후 앞의 'A'부터 덮어쒸워진다
    c_buf.push_front( 'G' );
    c_buf.push_front( 'H' );

    // --------------------------------------------------------------------------
    // 순환 버퍼의 메모리 주소 이미지
    //  ___ ___ ___ ___ ___
    // |_H_|_G_|_F_|_E_|_D_|
    // --------------------------------------------------------------------------
    std::for_each( c_buf.begin(), c_buf.end(), [] (char data) { std::cout << data << ' '; });
    std::cout << std::endl;

    // ------------------------------
    // 실제 메모리 주소를 확인
    // ------------------------------
    int num1 = c_buf.array_one().second;    // begin()에서 끝까지의 개수 '3'
    int num2 = c_buf.array_two().second;    // 실제 메모리 선두에서 end()까지의 개수는 '2'
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;

    // --------------------------------------------------------------------------
    // 실제 메모리 주소
    //  ___ ___ ___ ___ ___
    // |_E_|_D_|_H_|_F_|_G_|
    //       |   +--------------- 'H'가 begin() 위치, 이 위치를 포함해서 3개 연속
    //       +------------------- 'D'가 end() 위치. 선두 'E'를 포함해서 2개 연속2
    // --------------------------------------------------------------------------
    char* p1 = c_buf.array_one().first;
    char* p2 = c_buf.array_two().first;
    mem_dump( p1, num1 );                // begin()에서 실제 메모리 끝까지 출력
    mem_dump( p2, num2 );                // 실제 메모리 선두에서 end()까지 출력

    c_buf.pop_back();                    // 마지막을 삭제해본다.

    std::cout << std::endl;


    // --------------------------------------------------------------------------
    // 순환 버퍼의 메모리 주소 이미지
    //  ___ ___ ___ ___ ___
    // |_H_|_G_|_F_|_E_|___|
    // --------------------------------------------------------------------------
    std::for_each( c_buf.begin(), c_buf.end(), [] (char data) { std::cout << data << ' '; });
    std::cout << std::endl;

    num1 = c_buf.array_one().second;     
    num2 = c_buf.array_two().second;
    std::cout << num1  << std::endl;
    std::cout << num2  << std::endl;

    // --------------------------------------------------------------------------
    // 실제 메모리 주소
    //  ___ ___ ___ ___ ___
    // |_E_|___|_H_|_F_|_G_|
    //   |       +--------------- 'H'가 begin()위치. 이 위치를 포함해서 3개 연속
    //   +----------------------- 'E'가 end() 위치. 선두'E'를 포함해서 1개 연속
    // --------------------------------------------------------------------------
    p1 = c_buf.array_one().first;        
    p2 = c_buf.array_two().first;
    mem_dump( p1, num1 );
    mem_dump( p2, num2 );

    // linearize()을 사용하면 레거시 C의 모습으로 기록 가능
    mem_dump( c_buf.linearize(), 4 );

    return 0;
}