#include <iostream>
#include <boost/container/static_vector.hpp>

int main()
{
    boost::container::static_vector<int, 3> v;

    // 요소 추가
    v.push_back(3);
    v.push_back(1);
    v.push_back(4);

    // 모든 요소 출력
    for (auto x : v) 
	{
        std::cout << x << std::endl;
    }

    // 지정된 범위를 넘어서 추가를 하면 bad_alloc 예외 발생
	//v.push_back(5);
}