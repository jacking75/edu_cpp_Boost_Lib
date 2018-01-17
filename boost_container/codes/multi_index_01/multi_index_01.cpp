#include <iostream>
#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>

// 기본타입을 저장하는 기본적인 사용 방법 예제
int main()
{
	
	typedef boost::multi_index_container<std::string, // std::string을 저장
		boost::multi_index::indexed_by<
			// std::list 류로 입력 순서대로 접근
			boost::multi_index::sequenced<>,										
			// std::multiset 류로 정렬된 순서대로 접근. 정렬은 입력된 문자열의 값으로 
			boost::multi_index::ordered_non_unique< boost::multi_index::identity<std::string> > 
		>
	> Conainer;

	// sequenced
	Conainer ItemNameContainer;
	ItemNameContainer.push_back( "Knife" );
	ItemNameContainer.push_back( "Gun" );
	ItemNameContainer.push_back( "Pistol" );
	ItemNameContainer.push_back( "Sword" );
	ItemNameContainer.push_back( "Sword" );
	ItemNameContainer.push_back( "MiniGun" );
	ItemNameContainer.push_back( "Pistol" );

	std::cout << "ItemNameContainer의 모든 요소 출력" << std::endl;
	std::for_each( ItemNameContainer.begin(), ItemNameContainer.end(), [](const std::string& s) { 
						std::cout << s << std::endl;
					} );
	std::cout << std::endl;


	/// ordered_non_unique
	std::cout << "정렬된 ItemNameContainer의 모든 요소를 출력" << std::endl;
	Conainer::nth_index<1>::type& Index1 = ItemNameContainer.get<1>();
	std::for_each( Index1.begin(), Index1.end(), [](const std::string& s) { 
						std::cout << s << std::endl;
					} );
	std::cout << std::endl;


	// set 류로 사용 - O(logN)
	std::cout << "Pistol 개수 : " << Index1.count("Pistol") << std::endl;
	std::cout << std::endl;


	std::cout << "ItemNameContainer에서 Sword 요소 삭제" << std::endl;
	// set 류로 사용 - O(logN)
	Index1.erase("Sword");
		
	std::for_each( ItemNameContainer.begin(), ItemNameContainer.end(), [](const std::string& s) { 
						std::cout << s << std::endl;
					} );
	std::cout << std::endl;

	return 0;
}
