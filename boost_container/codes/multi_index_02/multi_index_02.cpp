#include <iostream>
#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

// 임의 접근 및 멤버 변수를 키로 사용

int main()
{
	
	typedef boost::multi_index_container<std::string, // std::string을 저장
		boost::multi_index::indexed_by<
			// std::vector 류로 입력 순서대로 접근
			boost::multi_index::random_access<>,										
			// std::multiset 류로 정렬된 순서대로 접근. 정렬은 입력된 문자열의 값으로 
			boost::multi_index::ordered_non_unique< boost::multi_index::identity<std::string> >, 
			/// 정렬은 문자열 길이로 																			
			boost::multi_index::ordered_non_unique< boost::multi_index::const_mem_fun<
															std::string, std::size_t, &std::string::length > > 
		>
	> Conainer;


	// sequenced - random access
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


	/// ordered_non_unique - 문자 순서 정렬
	std::cout << "문자 순서로 정렬된 ItemNameContainer의 모든 요소를 출력" << std::endl;
	
	Conainer::nth_index<1>::type& Index1 = ItemNameContainer.get<1>();
	std::for_each( Index1.begin(), Index1.end(), [](const std::string& s) { 
						std::cout << s << std::endl;
					} );
	std::cout << std::endl;


	/// ordered_non_unique - 문자 길이 정렬
	std::cout << "문자 길이로 정렬된 ItemNameContainer의 모든 요소를 출력" << std::endl;

	Conainer::nth_index<2>::type& Index2 = ItemNameContainer.get<2>();
	std::for_each( Index2.begin(), Index2.end(), [](const std::string& s) { 
						std::cout << s << std::endl;
					} );
	std::cout << std::endl;
	return 0;
}