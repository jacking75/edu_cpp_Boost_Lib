# Boost.DLL 
[공식 문서(1.73)](https://www.boost.org/doc/libs/1_73_0/libs/dll/ )  



## 플러그인 시스템을 Boost.DLL로 간단하게 구현하기
[출처](https://qiita.com/usagi/items/0722550dc5433bd07139 )  
  
### 준비할 것
1. plugin_type.hxx : 이 글에서 설명할 플러그인의 기본 타입 소스. 애플리케이션과 플러그인 각각에서 include 해서 사용한다.   
2. plugin_a.cxx : plugin_type을 기본으로 구현되는 구체적인 플러그인 소스.  
3. app.cxx : 플러그인을 사용하고 싶은 애플리케이션의 소스.  
  
<br>  

plugin_type.hxx:  
```
#pragma once

#include <string>

namespace usagi::example::boost_dll
{
  class plugin_type
  {
    public:
      virtual auto get_name() const -> std::string;
      virtual auto operator()( const int a, const int b ) const -> int;
      virtual ~plugin_type() { }
  };
}
```
  

plugin_a.cxx:  
```
#include "plugin_type.hxx"
#include <boost/config.hpp>

namespace usagi::example::boost_dll
{
  class plugin_a : public plugin_type
  {
    public:
      auto get_name() const -> std::string override { return "adder"; }
      auto operator()( const int a, const int b ) const -> int override { return a + b; };
  };

  extern "C" BOOST_SYMBOL_EXPORT plugin_a plugin;
  plugin_a plugin;
}
```
  

app.cxx:  
```
#include "plugin_type.hxx"
#include <boost/dll/import.hpp>
#include <iostream>

auto main() -> int
{
  using namespace std;
  using namespace usagi::example::boost_dll;
  // ↓ op는 boost::shared_ptr< plugin_type >  타입으로 주어진다.
  // （std::shared_ptr 로 변환하고 싶은 경우는 http://qiita.com/usagi/items/3563ddb01e4eb342485e를 참조한다.
  auto op =
    boost::dll::import< plugin_type >
    // ↓ 로드할 플러그인의 패스
    ( boost::filesystem::path( "plugin_a" )
    // ↓ import 할 대상의 플러그인으로 import 되고 있는 심볼
    , "plugin"
    // ↓ 이것을 붙여 두면 .so 나 .dll 을 패스에 자동으로 추가해 주므로 편리하다
    , boost::dll::load_mode::append_decorations
    );

  cout << op->get_name() << " " << (*op)( 1, 2 ) << endl;
}
```
  

### 응용: poker 플레이어 AI를 플러그인 화하여 놀수 있도록 한 예
[GitHub - usagi/poker](https://github.com/usagi/poker) 
  
poker/source/player_b.cxx:    
```
#include "player_type.hxx"

#include <boost/config.hpp>

#include <algorithm>

namespace usagi::poker
{
  using namespace std;
  
  class player_b: public player_type
  {
      auto get_name() -> std::string override
      {
        return "Player-B/Heart-man";
      }
      
      auto pay_ante( const size_t my_chips, const size_t minimum, const size_t maximum ) -> size_t override
      {
        if ( my_chips < 100 )
          return 1;
        else if ( my_chips < 1000 )
          return ( minimum + maximum ) / 2;
        return maximum;
      }
      
      auto discard_cards( const cards_type drew_cards ) -> cards_type override
      {
        // 러브 하트 알고리즘
        cards_type discards;
        
        copy_if
        ( drew_cards.cbegin()
        , drew_cards.cend()
        , back_inserter( discards )
        , []( const auto& card ) { return card.get_suit() != suit_type::heart; }
        );
        
        return discards;
      }
  };
  
  extern "C" BOOST_SYMBOL_EXPORT player_b player;
  player_b player;
}
```
  