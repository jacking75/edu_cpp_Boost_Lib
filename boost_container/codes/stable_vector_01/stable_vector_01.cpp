// 기존 vector와 성능 비교
#include <iostream>
#include <vector>
#include <boost/container/stable_vector.hpp>
#include <boost/timer/timer.hpp>

int main(int argc, char** argv) 
{
    int size = 10000;
    boost::timer::cpu_timer timer;

    timer.start();
    std::vector<int> vec(size);
    timer.stop();
    std::cout << timer.format();

    timer.start();
    boost::container::stable_vector<int> svec(size);
    timer.stop();
    std::cout << timer.format();



	//std::vector<int> v;

	//v.push_back(1);
	//auto const it = v.begin();
	//std::cout << *it << std::endl;


	//for (int i = 2; i < 10; ++i)
	//  v.push_back(i);

	//// ???
	//std::cout << *it << std::endl;

}
