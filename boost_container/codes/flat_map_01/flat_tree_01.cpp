#include <boost/container/flat_map.hpp> 


int main()
{
	{
		boost::container::flat_map<int, float> map;
		map.insert(std::make_pair(1, 1.1f));
		map.insert(std::make_pair(-1, -1.1f));
		map.insert(std::make_pair(3, 3.3f));
		map.insert(std::make_pair(2, 2.2f));

		std::cout << map[1] << std::endl;
	}

	{
		boost::container::flat_map<int, float> map;
		map.insert(std::make_pair(1, 1.1f));
		map.insert(std::make_pair(4, 4.4f));
		map.insert(std::make_pair(3, 3.3f));
		map.insert(std::make_pair(2, 2.2f));
	
		std::cout << map[4] << std::endl;
	}
	
	return 0;
}