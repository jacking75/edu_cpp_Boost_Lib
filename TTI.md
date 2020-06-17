# Boost.TTI
Boost.TTI는 어떤 타입이 멤버나 메소드를 가지고 있는지 아닌지를 판단하기 위한 라이브러리이다.
BOOST_MPL_HAS_XXX_TRAIT_DEF 의 확장판이라고 볼 수 있다.  
  
  
## 간단한 사용 방법
BOOST_MPL_HAS_XXX_TRAIT_DEF와 비슷하게 매크로 함수를 사용하여 메타 함수를 정의한다.    
```
#include <boost/tti/has_type.hpp>
#include <boost/tti/has_member_function.hpp>
#include <boost/tti/has_member_data.hpp>
#include <boost/mpl/assert.hpp>

namespace mpl = boost::mpl;


// has_xxx 를 정의한다
BOOST_TTI_HAS_TYPE(value_type)          // has_type_value_type
BOOST_TTI_HAS_MEMBER_DATA(value)        // has_member_data_value
BOOST_TTI_HAS_MEMBER_FUNCTION(func)     // has_member_function_funcg


struct X{
    typedef int value_type;
    value_type value;

    void func(int, int){}
    int func(float){ return 0; }
};


// value_type 유무
BOOST_MPL_ASSERT((has_type_value_type<X>));

// value_type의 타입을 지정하고 체크도 가능하다
BOOST_MPL_ASSERT    ((has_type_value_type<X, int>));
BOOST_MPL_ASSERT_NOT((has_type_value_type<X, void>));


// 프로퍼티 value 유무
// value의 타입을 지정한다
BOOST_MPL_ASSERT    ((has_member_data_value<X, X::value_type>));
BOOST_MPL_ASSERT_NOT((has_member_data_value<X, float>));


// 메소드 func 유무
// 반환 값 타입과 인수 타입을 지정한다
BOOST_MPL_ASSERT((
    has_member_function_func<
        X,
        void,                    // 반환 값 타입
        mpl::vector<int, int>    // 인수는 mpl::vector 로 넘긴다
    >
));

// 오버로드 하고 있어도 체크 할 수 있다
BOOST_MPL_ASSERT((
    has_member_function_func<
        X,
        int,
        mpl::vector<float>
    >
));

BOOST_MPL_ASSERT_NOT((has_member_function_func<X, void>));

int main(){}
```
  
이런 식으로 정의한 메타 함수에 타입을 넘겨서 사용한다.  
또 BOOST_TTI_TRAIT_{macro name} 로 메타 함수 이름을 지정할 수도 있다.  
```
// has_value 라는 메타 함수 이름으로 정의
BOOST_TTI_TRAIT_HAS_MEMBER_DATA(has_value, value)

struct X{
    int value;
};

BOOST_MPL_ASSERT((boost::tti::has_value<X, int>));
```
   
   
### ::type을 전개
메타 프로그래밍을 하고 있을 때 아래와 같이 `::type`을 전개해가 바라는 경우가 있다.  
```
#include <boost/tti/has_type.hpp>

BOOST_TTI_HAS_TYPE(value_type)

template<typename T>
struct element_value_type{
    typedef typename T::value_type type;
};

template<typename T>
struct X
    // <mpl::identity<T>::type, element_type<T>::type> 라고 전개하기 바란다
    : has_type_value_type<mpl::identity<T>, element_value_type<T> >{};
```  
    
안타깝게 `has_type_value_type` 내에서는 `::type` 전개는 할 수 없다.  
여기에서 Boost.TTI 에서 준비한 헬퍼 함수를 사용한다.  
```
#include <boost/tti/has_type.hpp>
// 추가
#include <boost/tti/mf/mf_has_type.hpp>

BOOST_TTI_HAS_TYPE(value_type)

template<typename T>
struct element_value_type{
    typedef typename T::value_type type;
};

template<typename T>
struct X
    // boost::tti::mf_has_type
    // Boost.MPL의 placeholders를 사용하여 부분 적용된다
    : boost::tti::mf_has_type<
        has_type_value_type<mpl::_1, mpl::_2>,
        mpl::identity<T>,
        element_value_type<T>
    >
{};
```  
    
이것으로 `has_type_value_type`에 `::type`을 전개한 결과가 적용된다.   
   
    
### 실제로 사용해보기
다음과 같은 함수를 정의해 본다.  
1. T가 value_type 타입의 value를 멤버로서 가지고 있는 경우는 value를 반환  
2. 이 이외의 경우는 T를 반환  
  
```
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/mpl/and.hpp>
#include <boost/tti/has_type.hpp>
#include <boost/tti/mf/mf_has_type.hpp>
#include <boost/tti/has_member_data.hpp>
#include <boost/tti/mf/mf_has_member_data.hpp>

namespace mpl = boost::mpl;

BOOST_TTI_HAS_TYPE(value_type)           // has_type_value_type
BOOST_TTI_HAS_MEMBER_DATA(value)         // has_member_data_value

template<typename T>
struct element_value_type{
    typedef typename T::value_type type;
};

template<typename T>
struct trivial_get_value :
    mpl::and_<
        boost::is_class<T>,
        has_type_value_type<T>,
        boost::tti::mf_has_member_data<
            has_member_data_value<mpl::_1, mpl::_2>,
            mpl::identity<T>,
            element_value_type<T>
        >
    >::type
{};

template<typename T>
typename T::value_type
get_value(T const& t, typename boost::enable_if<trivial_get_value<T> >::type* =0){
    return t.value;
}

template<typename T>
T const&
get_value(T const& t, typename boost::disable_if<trivial_get_value<T> >::type* =0){
    return t;
}


struct X{
    typedef int value_type;
    value_type value;
};

struct hoge{};


int
main(){
    X x = {4};
    int n = get_value(x);
    
    hoge h;
    get_value(h);
    get_value(0);
    return 0;
}
```   
	  
`trivial_get_value` 에서 `get_value` 기능을 만족하고 있는지를 체크하고 있다.  
  
### 주의
Boost.TTI 의 메타 함수에 클래스 이외의 타입을 넘기면 컴파일 에러(false를 반환하는 것이 아닌 )가 되므로 주의한다.  
  
	  
	  
## 멤버를 가지고 있는지 조사하는 예제 코드 
  
```
#include <boost/tti/has_member_function.hpp>


BOOST_TTI_HAS_MEMBER_FUNCTION(close)
// => has_member_function_close

struct custom_deleter{
    template<typename T>
    void
    operator ()(T* p,
        typename std::enable_if<
            has_member_function_close<void (T::*)()>::value
        >::type* = 0
    ) const{
        p->close();
        delete p;
    }
    template<typename T>
    void
    operator ()(T* p,
        typename std::enable_if<
            has_member_function_close<void (T::*)(int)>::value
        >::type* = 0
    ) const{
        p->close(100);
        delete p;
    }
};


#include <memory>
#include <type_traits>
#include <iostream>


struct Hoge{
    Hoge() {}
    ~Hoge() {}
    void close() {
        std::cout << "Hoge::close" << std::endl;
    };
};

struct Fuga{
    Fuga() {}
    ~Fuga() {}
    void close(int i) {
        std::cout << "Fuga::close" << std::endl;
    };
};


int
main(){
    {
        std::shared_ptr<Hoge> hoge(new Hoge(), custom_deleter());
        std::shared_ptr<Fuga> fuga(new Fuga(), custom_deleter());
    }
    return 0;
}
```  
	  
	  
## C++03에서 특정 멤버 함수가 존재하는지를 판정
Boost.TTI의 `has_member_function` 기능을 사용하여 특정 멤버 함수를 가지고 있는지 판정한다.  
C++11의 SFINAE Expression를 사용할 수 없는 경우에 유용하다.  
```
#include <boost/tti/has_member_function.hpp>

struct X {
    int foo(double, char);
};

namespace my_namespace {
    // foo() 멤버 함수를 가지고 있는지 판정하기 위한 메타 함수를 정의한다
    BOOST_TTI_HAS_MEMBER_FUNCTION(foo)
}

int main()
{
    static_assert(
        my_namespace::has_member_function_foo<
            X,                               // 대상의 타입
            int,                             // 반환 값 타입
            boost::mpl::vector<double, char> // 파라메터 타입
         >::value == true,
        "must be true");
}  
```  
  
	  
## 위 글들의 출처
- https://osyo-manga.hatenadiary.org/entry/20111230/1325213907