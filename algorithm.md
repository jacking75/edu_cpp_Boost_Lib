# Algorithm
  
## clamp()
수치 변화를 지정 범위 내로 하기  
hp의 값이 최소 0 ~ maxHP 사이가 되도록 한다.  
  
```
#include <boost/algorithm/clamp.hpp>

void hpchange(int addhp){
  hp = boost::algorithm::clamp(hp + addhp, 0, maxHp);
}
```
  

## clamp_range()  
clamp()에 의한 값의 변환 처리를 값의 Range에 대응한다.  
이 함수는 OutputIterator로 변환 후의 Range을 반환한다.  
```
#include <iostream>
#include <vector>
#include <boost/algorithm/clamp.hpp>

int main()
{
    std::vector<int> v = {-1, 2, 5, 8, 11};

    // Rangeの値を0から10の範囲に収める
    boost::algorithm::clamp_range(v, v.begin(), 0, 10);

    for (int x : v) {
        std::cout << x << std::endl;
    }
}
```    
결과:   
<pre>
0
2
5
8
10
</pre>
  

## all_of
C++11의 all_of의 Range 버전   
```
#include <iostream>
#include <vector>
#include <boost/algorithm/cxx11/all_of.hpp>

bool is_even(int x) { return x % 2 == 0; }

int main()
{
    const std::vector<int> v = {2, 4, 6, 8, 10};

    std::cout << std::boolalpha;

    // 이터레이터 버전
    {
        const bool result = boost::algorithm::all_of(v.begin(), v.end(), is_even);
        std::cout << result << std::endl;
    }

    // Range 버전
    {
        const bool result = boost::algorithm::all_of(v, is_even);
        std::cout << result << std::endl;
    }
}
```  
결과:    
<pre>
true
true
</pre>
     
  
## hex()
16진수 표현의 문자열을 10진수 표현으로 바꾼다.  
```
#include <iostream>
#include <string>
#include <iterator>
#include <boost/algorithm/hex.hpp>

int main()
{
    std::string s = "abc";

    std::string out;
    boost::algorithm::hex(s, std::back_inserter(out));

    std::cout << out << std::endl;
}
```  
결과:      
<pre>
616263
</pre>
  

## unhex()
10 진수 표현 문자열을 16진수 표현으로 바꾼다.  
```
#include <iostream>
#include <string>
#include <iterator>
#include <boost/algorithm/hex.hpp>

int main()
{
    std::string s = "616263";

    std::string out;
    boost::algorithm::unhex(s, std::back_inserter(out));

    std::cout << out << std::endl;
}
```  
결과:  
<pre>
abc
</pre>
  


