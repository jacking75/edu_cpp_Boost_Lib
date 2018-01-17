#include <iostream>
#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
#include <boost/tuple/tuple.hpp>

// modify 사용

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
typedef boost::multi_index::const_mem_fun<CHARACTER, const int, &CHARACTER::Level> IDX_LEVEL;
typedef boost::multi_index::member<CHARACTER, int, &CHARACTER::m_nID> IDX_CHARID;

// 인덱싱 타입을 선언
typedef struct indices : public boost::multi_index::indexed_by
	<
		boost::multi_index::hashed_unique<IDX_CHARID>
		, boost::multi_index::hashed_non_unique<IDX_LEVEL>
	>
{
	enum INDEX
	{
		IDX_UNIQUE_CHARID
		, IDX_NON_UNIQUE_LEVEL 
		, IDX_END		
	};
} INDICES;

typedef boost::multi_index_container<CHARACTER, indices> Container;


int main() 
{
	Container CharacterSet;

	CharacterSet.insert( CHARACTER(1, 2, 21));
	CharacterSet.insert( CHARACTER(2, 3, 31));
	CharacterSet.insert( CHARACTER(3, 23, 39));
	CharacterSet.insert( CHARACTER(4, 3, 35));
	CharacterSet.insert( CHARACTER(5, 1, 11));
	CharacterSet.insert( CHARACTER(6, 1, 12));
	CharacterSet.insert( CHARACTER(7, 1, 10));
	CharacterSet.insert( CHARACTER(8, 4, 41));
	

	std::cout << "모든 캐릭터 출력" << std::endl;
	std::for_each( CharacterSet.begin(), CharacterSet.end(), [](const CHARACTER& Char) { 
		std::cout << "CharID : " << Char.m_nID << ", Level : " << Char.m_nLevel << ", Exp : " << Char.m_nExp << std::endl;
					} );
	std::cout << std::endl;


	/// modify
	std::cout << "캐릭터 ID 3번을 33번으로 그리고 레벨도 333으로 변경" << std::endl;

	Container::nth_index<indices::IDX_UNIQUE_CHARID>::type &Index1 = CharacterSet.get<indices::IDX_UNIQUE_CHARID>();
	Container::nth_index_iterator<indices::IDX_UNIQUE_CHARID>::type iter = Index1.find(3);

	if (Index1.end() != iter) 
	{
		std::cout << "CharID : " << (*iter).m_nID << std::endl;
		Index1.modify( iter, [](CHARACTER& Char) { Char.m_nID = 33; Char.m_nLevel = 333; } );
	}
	std::cout << std::endl;


	std::for_each( CharacterSet.begin(), CharacterSet.end(), [](const CHARACTER& Char) { 
		std::cout << "CharID : " << Char.m_nID << ", Level : " << Char.m_nLevel << ", Exp : " << Char.m_nExp << std::endl;
					} );
	std::cout << std::endl << std::endl;




	/// modify rollback
	std::cout << "캐릭터 ID 4번을 6번으로 그리고 레벨도 666으로 변경" << std::endl;

	iter = Index1.find(4);

	if (Index1.end() != iter) 
	{
		std::cout << "CharID : " << (*iter).m_nID << "번을 6번, 레벨은 666으로 변경" << std::endl;
		int nCharID = (*iter).m_nID;
		int nLevel = (*iter).m_nLevel;
		Index1.modify( iter, [](CHARACTER& Char) { Char.m_nID = 6; Char.m_nLevel = 666; }, [nCharID, nLevel](CHARACTER& Char) { Char.m_nID = nCharID; Char.m_nLevel = nLevel; } );
	}
	std::cout << std::endl;


	std::for_each( CharacterSet.begin(), CharacterSet.end(), [](const CHARACTER& Char) { 
		std::cout << "CharID : " << Char.m_nID << ", Level : " << Char.m_nLevel << ", Exp : " << Char.m_nExp << std::endl;
					} );
	std::cout << std::endl << std::endl;



	/// modify key
	std::cout << "캐릭터 ID 33번을 333333번으로 변경" << std::endl;

	iter = Index1.find(33);

	if (Index1.end() != iter) 
	{
		std::cout << "CharID : " << (*iter).m_nID << std::endl;
		Index1.modify_key( iter, [](int& nCharID) { nCharID = 333333; } );
	}
	std::cout << std::endl;


	std::for_each( CharacterSet.begin(), CharacterSet.end(), [](const CHARACTER& Char) { 
		std::cout << "CharID : " << Char.m_nID << ", Level : " << Char.m_nLevel << ", Exp : " << Char.m_nExp << std::endl;
					} );
	std::cout << std::endl;

	return 0;
}