#include <iostream>
#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
#include <boost/tuple/tuple.hpp>

// 합성 키와 바운드리 검색

class CHARACTER 
{
public:
	CHARACTER( const int nID, const int nLevel, const int nExp ) : m_nID(nID), m_nLevel(nLevel), m_nExp(nExp)
	{
	}

	const int Level() const { return m_nLevel; }
	const int Exp() const { return m_nExp; }

	int m_nLevel;
	int m_nExp;
	int m_nID;
};

// 컨테이너의 키를 선언
typedef boost::multi_index::const_mem_fun<CHARACTER, const int, &CHARACTER::Level>	IDX_LEVEL;
typedef boost::multi_index::member<CHARACTER, int, &CHARACTER::m_nExp>				IDX_EXP;
typedef boost::multi_index::composite_key<CHARACTER, IDX_LEVEL, IDX_EXP>			IDX_LEVEL_EXP;
typedef boost::multi_index::composite_key<CHARACTER, IDX_EXP, IDX_LEVEL>			IDX_EXP_LEVEL;
typedef boost::multi_index::member<CHARACTER, int, &CHARACTER::m_nID>				IDX_CHARID;

// 인덱싱 타입을 선언
typedef struct indices : public boost::multi_index::indexed_by
	<
		boost::multi_index::ordered_non_unique<IDX_LEVEL_EXP>
		, boost::multi_index::hashed_non_unique<IDX_EXP_LEVEL>
		, boost::multi_index::hashed_unique<IDX_CHARID>
		, boost::multi_index::hashed_non_unique<IDX_LEVEL>
	>
{
	enum INDEX
	{
		IDX_NON_UNIQUE_LEVEL_EXP = 0				
		, IDX_NON_UNIQUE_EXP_LEVEL 
		, IDX_UNIQUE_CHARID
		, IDX_NON_UNIQUE_LEVEL 
		, IDX_END		
	};
} INDICES;

typedef boost::multi_index_container<CHARACTER, indices> Container;


int main() 
{
	Container CharacterSet;

	CharacterSet.insert( CHARACTER(1, 2, 21) );
	CharacterSet.insert( CHARACTER(2, 3, 31) );
	CharacterSet.insert( CHARACTER(3, 3, 39) );
	CharacterSet.insert( CHARACTER(4, 3, 35) );
	CharacterSet.insert( CHARACTER(5, 1, 11) );
	CharacterSet.insert( CHARACTER(6, 1, 12) );
	CharacterSet.insert( CHARACTER(7, 1, 10) );
	CharacterSet.insert( CHARACTER(8, 4, 41) );
	
	std::cout << "정렬된 LEVEL-EXP 인덱스로 모든 캐릭터 출력" << std::endl;
	Container::nth_index<indices::IDX_NON_UNIQUE_LEVEL_EXP>::type& Index1 = CharacterSet.get<indices::IDX_NON_UNIQUE_LEVEL_EXP>();
	std::for_each( Index1.begin(), Index1.end(), [](const CHARACTER& Char) { 
				std::cout << "Level : " << Char.Level() << ", Exp : " << Char.Exp() << std::endl;
					} );
	std::cout << std::endl << std::endl;



	std::cout << "정렬되지 않은 EXP-LEVEL 인덱스로 모든 캐릭터 출력" << std::endl;

	Container::nth_index<indices::IDX_NON_UNIQUE_EXP_LEVEL>::type& Index2 = CharacterSet.get<indices::IDX_NON_UNIQUE_EXP_LEVEL>();
	std::for_each( Index2.begin(), Index2.end(), [](const CHARACTER& Char) { 
				std::cout << "Level : " << Char.Level() << ", Exp : " << Char.Exp() << std::endl;
					} );
	std::cout << std::endl << std::endl;
	


	// bound
	std::cout << "(1, 10 ) <= CHARACTER < (3, 39)" << std::endl;

	boost::tuple<const int, const int> GeqKey(1,10); // >=
	boost::tuple<const int, const int> LtKey(3, 39); // <

	Container::iterator kb = CharacterSet.get<indices::IDX_NON_UNIQUE_LEVEL_EXP>().lower_bound( GeqKey );
	Container::iterator ke = CharacterSet.get<indices::IDX_NON_UNIQUE_LEVEL_EXP>().lower_bound( LtKey );
	while( kb != ke ) 
	{
	  std::cout << "Level : " << (*kb).Level() << ", Exp : " << (*kb).Exp() << std::endl;
	  ++kb;
	}
	std::cout << std::endl << std::endl;


	
	/// find
	std::cout << "캐릭터 ID 3번 검색" << std::endl;

	Container::nth_index<indices::IDX_UNIQUE_CHARID>::type &Index3 = CharacterSet.get<indices::IDX_UNIQUE_CHARID>();
	Container::nth_index_iterator<indices::IDX_UNIQUE_CHARID>::type iter = Index3.find(3);

	if (Index3.end() != iter) 
	{
		std::cout << "CharID : " << (*iter).m_nID << std::endl;
	}
	std::cout << std::endl << std::endl;


	/// find 2
	std::cout << "캐릭터 레벨 3 검색" << std::endl;

	Container::nth_index<indices::IDX_NON_UNIQUE_LEVEL>::type &Index4 
		= CharacterSet.get<indices::IDX_NON_UNIQUE_LEVEL>();

	std::pair<
		Container::nth_index_iterator<indices::IDX_NON_UNIQUE_LEVEL>::type, 
		Container::nth_index_iterator<indices::IDX_NON_UNIQUE_LEVEL>::type> 
		pair;

	pair = Index4.equal_range(3);
	for ( auto iter = pair.first; iter != pair.second; ++iter)
	{
		std::cout << "CharID : " << (*iter).m_nID << ", Level : " << (*iter).m_nLevel << std::endl;
	}


	return 0;
}