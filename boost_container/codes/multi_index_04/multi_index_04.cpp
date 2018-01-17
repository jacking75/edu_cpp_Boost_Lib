#include <iostream>
#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
#include <boost/tuple/tuple.hpp>

// 유저 정의형 자료를 저장.

class Character 
{
public:
	Character( const int nCharID, const std::wstring strName ) : m_nCharID(nCharID), m_strName(strName)
	{
		m_nExp = 0;
		m_nMoney = 0;
	}
		
	int m_nCharID;
	std::wstring m_strName;
	int m_nExp;
	int m_nMoney;
};

// 컨테이너의 키를 선언
typedef boost::multi_index::member<Character, int, &Character::m_nCharID>			IDX_CHARID;
typedef boost::multi_index::member<Character, std::wstring, &Character::m_strName>	IDX_NAME;

// 인덱싱 타입을 선언
typedef struct indices : public boost::multi_index::indexed_by
	<
		boost::multi_index::hashed_unique<IDX_CHARID>
		, boost::multi_index::hashed_unique<IDX_NAME>
	>
{
	enum INDEX
	{
		IDX_UNIQUE_CHARID		= 0
		, IDX_UNIQUE_NAME		 
		, IDX_END		
	};
} INDICES;

typedef boost::multi_index_container<Character, indices> Container;


int main() 
{
	setlocale(LC_ALL, "");

	Container CharacterSet;

	CharacterSet.insert( Character(1, L"최흥배") );
	CharacterSet.insert( Character(2, L"임영기") );
	CharacterSet.insert( Character(3, L"이욱진") );
	
	
	std::cout << "캐릭터 ID 2번 찾기" << std::endl;
	Container::nth_index<indices::IDX_UNIQUE_CHARID>::type &Index1 = CharacterSet.get<indices::IDX_UNIQUE_CHARID>();
	Container::nth_index_iterator<indices::IDX_UNIQUE_CHARID>::type iter1 = Index1.find(2);

	if (Index1.end() != iter1) 
	{
		std::wcout << "CharID : " << (*iter1).m_nCharID << ", Name : " << (*iter1).m_strName << std::endl;
	}
	else
	{
		std::wcout << "캐릭터 ID 2번을 찾을 수 없습니다" << std::endl;
	}

	std::cout << std::endl << std::endl;



	std::cout << "캐릭터 이름 이욱진 찾기" << std::endl;

	Container::nth_index<indices::IDX_UNIQUE_NAME>::type &Index2 = CharacterSet.get<indices::IDX_UNIQUE_NAME>();
	Container::nth_index_iterator<indices::IDX_UNIQUE_NAME>::type iter2 = Index2.find(L"이욱진");

	if (Index2.end() != iter2) 
	{
		std::wcout << "CharID : " << (*iter2).m_nCharID << ", Name : " << (*iter2).m_strName << std::endl;
	}
	else
	{
		std::wcout << "캐릭터 이름 '이욱진'을 찾을 수 없습니다" << std::endl;
	}

	return 0;
}