#include <iostream>
#include <boost/container/static_vector.hpp>

int main()
{
    boost::container::static_vector<int, 5> v;

    // 요소 추가
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
	v.push_back(4);
    
	std::cout << "사이즈: " << v.size() << ", 허용크기:" << v.capacity() << std::endl;
	std::for_each( v.begin(), v.end(), [] (int data) { std::cout << data << ' '; });
	std::cout << std::endl;


    v.resize(3);
	std::cout << "사이즈: " << v.size() << ", 허용크기:" << v.capacity() << std::endl;
	std::for_each( v.begin(), v.end(), [] (int data) { std::cout << data << ' '; });
	std::cout << std::endl;
}