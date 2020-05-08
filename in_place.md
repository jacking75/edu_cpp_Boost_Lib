# boost::in_place 
  
[출처](https://nekko1119.hatenablog.com/entry/20121122/1353577143 )  
`boost::in_place`를 사용하면 확복한 메모리에 오브젝트를 뒤에서 만들 수 있다.  
copy 불가한 오브젝트도 재 구축으로 재 대입하는 행동을 한다.  
  
trivial destructor를 가진 클래스는 소멸하지 않고 재 구축 할 수 있다.  
    
```
#include <iostream>
#include <string>
#include <sstream>
#include <boost/utility/in_place_factory.hpp>

template<class T>
class hoge
{
public:

    //T 타입 분의 메모리를 확보한다
    hoge() : buffer_(reinterpret_cast<T*>(new char[sizeof(T)]))
    {
    }

    //메모리 해제
    ~hoge()
    {
        delete buffer_;
    }

    //확보한 메모리에 오브젝트를 확보한다
    template<class InPlace>
    void construct(const InPlace& p)
    {
        p.template apply<T>(buffer_);
    }

    //오브젝트 파괴
    void destroy()
    {
        buffer_->~T();
    }

    std::string to_string() const
    {
        return buffer_->to_string();
    }    

private:
    T* buffer_;
};

class point
{
public:
    point(int x, int y) : x_(x), y_(y)
    {
    }

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "(" << x_ << ", " << y_ << ")";
        return oss.str();
    }

private:
    int x_;
    int y_;
};

int main()
{
    hoge<point> a;// 메모리 확보
    a.construct(boost::in_place(100, 200));//오브젝트 구축
    std::cout << a.to_string() << std::endl;

    //point 타입은 trivial destructor 이므로 소멸하지 않고 재 구축 할 수 있다

    a.construct(boost::in_place(300, 400));//재 구축
    std::cout << a.to_string() << std::endl;

    return 0;
}
```
  
실행 결과: 
<pre>
(100, 200)
(300, 400)
</pre>  
  