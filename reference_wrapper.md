# 참조를 보관하는 컨테이너
[출처](https://faithandbrave.hateblo.jp/entry/20110519/1305789940 )  
  
같은 기본 클래스를 승계한 클래스의 오브젝트가 몇개 있고, 이것들을 모아서 사용하고 싶은 경우와 전문특화해서 다루고 싶은 경우, 양쪽을 동시에 필요에 경우가 있다.  
이럴 때 오브젝트의 참조를 보관하는 컨테이너가 필요하게 되는데 boost::reference_wrapper 컨테이너를 사용하면 좋다.  
  
```
#include <iostream>
#include <string>
#include <vector>
#include <boost/ref.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>

struct Base {
    // 공통으로 사용하는 함수
    void print() const { std::cout << "print" << std::endl; }
};

struct Integer : Base {
    // 전문 특화한 함수
    int get_int() const { return 3; }
};

struct String : Base {
    // 전문 특화한 함수
    std::string get_string() const { return "Hello"; }
};

int main()
{
    Integer n;
    String s;

    std::vector<boost::reference_wrapper<Base> > v;
    v.push_back(boost::ref<Base>(n));
    v.push_back(boost::ref<Base>(s));

    // 공통 함수를 사용한다
    BOOST_FOREACH (Base& base, v) {
        base.print();
    }

    // 전문 특화한 함수를 사용한다
    const int v1 = n.get_int();
    const std::string v2 = s.get_string();
}
```
  
결과  
<pre>
print
print
</pre>  
  