#include <iostream>
#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

// typedef를 사용하여 멀티인덱스 선언. 사용할 때 직관적으로 사용할 수 있다

int main()
{
	// 컨테이너의 키를 선언
	typedef boost::multi_index::random_access<> Index_RandomAccess;
	typedef boost::multi_index::identity<std::string> Index_Ordered_Non_Unique;
	typedef boost::multi_index::const_mem_fun< std::string, std::size_t, &std::string::length > Index_Ordered_Non_Unique_Str_Length;

	// 인덱싱 타입을 선언
	typedef struct indices : public boost::multi_index::indexed_by
		<
			Index_RandomAccess
			, boost::multi_index::ordered_non_unique<Index_Ordered_Non_Unique>
			, boost::multi_index::ordered_non_unique<Index_Ordered_Non_Unique_Str_Length>
		>
	{
		enum INDEX
		{
			IDX_RANDOM_ACCESS					= 0
			, IDX_NON_UNIQUE				
			, IDX_NON_UNIQUE_STR_LENGTH	
			, IDX_END		
		};
	} INDICES;

	typedef boost::multi_index_container<std::string, indices> Conainer;


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

	Conainer::nth_index<indices::IDX_NON_UNIQUE>::type& Index1 = ItemNameContainer.get<indices::IDX_NON_UNIQUE>();
	std::for_each( Index1.begin(), Index1.end(), [](const std::string& s) { 
						std::cout << s << std::endl;
					} );
	std::cout << std::endl;



	/// ordered_non_unique - 문자 길이 정렬
	std::cout << "문자 길이로 정렬된 ItemNameContainer의 모든 요소를 출력" << std::endl;

	Conainer::nth_index<indices::IDX_NON_UNIQUE_STR_LENGTH>::type& Index2 = ItemNameContainer.get<indices::IDX_NON_UNIQUE_STR_LENGTH>();
	std::for_each( Index2.begin(), Index2.end(), [](const std::string& s) { 
						std::cout << s << std::endl;
					} );
	std::cout << std::endl;
	
	return 0;
}