# 독자적인 할당기 사용하기
[출처](http://www.6809.net/tenk/?%E9%9B%91%E8%A8%98%2f2012-12-14   )  
  
  
## vector
  
```
template <class Allocator>
class vector_alloc_holder {
    struct members_holder : public Allocator {
        pointer m_start;
        size_type m_size;
        size_type m_capacity;
    }
}
  
template <class T, class Allocator>
class vector : vector_alloc_holder <Allocator>;
```
  
Allocator는 (비 static) 멤버 변수가 없으면 실질적으로는 0 바이트가 되도록 struct의 부모로 되어 있다.  
stl의 구현에서는 약간의 차위는 있는 Allocator는 (비 static) 멤버 변수가 있는 것이 많지만, C++03의 경우 static 멤버 변수로 갖는 것도 있듯이, 실제로 그렇게된 컴파일러도 있었다.  
C++11에서는 scoped_allocator_adaptor도 있고, Allocator는 (비 static)멤버 변수기 필수인 것 같다.  
  
C++11 이라면  
```
// 1회 N개 할당하는 Allocator
template<typename T, unsigned N>
class SampleAllocator1 {
public:
   typedef T value_type;
   SampleAllocator1() { }
   T* allocate(unsigned n) { assert(n == N); return reinterpret_cast<T*>(buf_); }
   void  deallocate(T*, unsigned) { }
   bool operator == (SampleAllocator1 const & ) const { return false; }
   bool operator != (SampleAllocator1 const & ) const { return true; }
private:
   typedef double buf_t;
   buf_t   buf_[(N*sizeof(T)+sizeof(buf_t)-1) / sizeof(buf_t)];
};
//typedef std::vector<int, SampleAllocator1<int,1024> >  Int1024Vector;
typedef boost::container::vector<int, SampleAllocator1<int,1024> >  Int1024Vector;
Int1024Vector  g_intVec;
```
  
위처럼 해도 괜찮을 듯 하다. 단 boost::container에 의존하고 있다.  
  
  
  
## list
  
```
class Node_Base {
    Node_Base* next_;
    Node_Base* prev_;
};
template<typename T>
class Node : Node_Base {
    T     value_;
};
template<typename T, class A >
class List : A {
    ListNode_Base root_;
    size_t        size_;
};
```
  
  
  
## map, multimap, set multiset
map, set은 대부분 레드-블랙 트리 구현과 같고, boost::container는 boost::intrusive를 사용하고 있는 것 같다.  
  
```
class Node_Base {
    Node_Base* left_;
    Node_Base* right_;
    Node_Base* parent_;
    bool       balance_;
};
template<typename T>
class Node : Node_Base {
    T     value_;
};
template<typename T >
class Tree : A {
    Node_Base root_;
    size_t    size_;
};
```
  
List와 Map은 전달된 할당을 그대로 사용하는 것이 아니라, allocator의 멤버  
```
template <class U> struct rebind { typedef allocator<U> other; };
```
  
를 사용하여 T가 아닌 Node<T> allocator를 사용하고 있다.   
또한 allocator에 대한 요청은 일반적으로 1개 단위 라고 생각한다.  
  
그래서 list 나 map의 나의 할당기는 노드 크기 고정의 메모리 풀을 둔다, 라고 생각한다.  
```
template<unsigned B, unsigned N>
class SampleAlloc2 {
public:
    SampleAlloc2() {
        for (unsigned i = 0; i < N-1; ++i)
            buf_[i].link = &buf_[i+1];
        buf_[N-1].link = NULL;
        root_ = &buf_[0];
    }
    void* allocate(unsigned n) {
        assert(n == 1 && root_);
        Link*   p = root_;
        root_ = p->link;
        return p;
    }
    void  deallocate(void* t, unsigned n) {
        if (t) {
            assert(n == 1);
            Link*   p = reinterpret_cast<Link*>(t);
            p->link   = root_;
            root_     = p;
        }
    }
private:
    union Link {
        Link*   link;
        char    b[B];
    };
private:
    Link*       root_;
    Link        buf_[N];
};
enum { ELM_SIZE = 32 }; // コンテナのノードを含んだ要素のバイト数.
enum { MAX_SIZE = 16 };
template<typename T>
class SampleAllocator2 : public SampleAlloc2<ELM_SIZE,MAX_SIZE> {
    typedef SampleAlloc2<ELM_SIZE,MAX_SIZE> base_type;
public:
    typedef T value_type;
    SampleAllocator2() {BOOST_STATIC_ASSERT(sizeof(T) <= ELM_SIZE);}
    T* allocate(unsigned n) { return reinterpret_cast<T*>(base_type::allocate(n)); }
    void  deallocate(T* t, unsigned n) { base_type::deallocate(t, n); }
    bool operator == (SampleAllocator2 const & ) const { return false; }
    bool operator != (SampleAllocator2 const & ) const { return true; }
};
// list
typedef SampleAllocator2<int>  IntAllocator;
typedef boost::container::list<int, IntAllocator > IntList;
// map
struct cstr_less {
    bool operator()(const char* l, const char* r) const { return strcmp(l,r) < 0; }
};
typedef std::pair<char const* const,int> smpPair_t;
typedef SampleAllocator2<smpPair_t> SmpPairAllocator;
typedef boost::container::map<const char*, int, cstr_less, SmpPairAllocator >	StrIntMap;
```
  
  
  
## string
std::string은 (C++03 에서) 라이브러리에 마다 상당히 구현이 다르다(EffectiveSTL에 실려 있는 듯).  
최소한으로 vector와 같은 내용이 될 것이지만... boost::container::string에서   
```
struct long_t {
    size_type      is_short  : 1;
    size_type      length    : (sizeof(size_type)*CHAR_BIT - 1);
    size_type      storage;
    pointer        start;
 };
 struct short_header {
    unsigned char  is_short  : 1;
    unsigned char  length    : (CHAR_BIT - 1);
 };
 struct short_t {
    short_header   h;
    value_type     data[UnalignedFinalInternalBufferChars]; 
                   // UnalignedFinalInternalBufferChars ≒ (sizeof(long_t)-sizeof(short_header))/sizeof(value_type)
 };
 union repr_t {
    long_raw_t  r;
    short_t     s;
 };
```
  
long_t도 긴 문자열 할당을 사용하는 경우, sizeof(long_t) -α 이내로 하고, 짧은 경우 short_t으로 영역을 버퍼로 사용하여 동적 확보하지 않고 끝내는 궁리로 되어 있다. 64bit CPU 라면, vector 호환에서도 `sizeof(포인터) * 3 = 24bytes` 로 꽤 고마운 구현이다.   
  
※ 참고: 비트 필드는 엔디언에 관계 없이 먼저 쓰여진 것이 낮은 주소에 배치된다.  
  
  
  
## scoped_allocator_adaptor
C++11에서 추가된 할당기이다.  
scoped_allocator_adaptor을 이용하면, 예를 들면 map<string, int> 같은 컨테이너에서 로컬 할당을 string과 map에서 공통으로 사용할 수 있도록 할 수 있다.  
※ 내측의 컨테이너(이 경우 string)의 생성자로 기본 생성자가 아닌 할당기를 인수로 취하는 생성자가 사용된다.  
  
할당기를 컨테이너의 멤버로 하기 때문에, 당연히 메모리 사용량도 증가(특히 안쪽의 컨테이너는 수가 많아지기 쉽다). 때문에 컨테이너의 멤버로 하는 할당기는 포인터만으로 하고, 구현은 그 앞에 준비한다는 느낌이다.  
  
자세한 것은 [이쪽 사이트](http://d.hatena.ne.jp/faith_and_brave/20081128/1227867484 )등을 보기 바란다.  

아래는 테스트용 코드  
```
#include <stdio.h>
#include <boost/container/map.hpp>
#include <boost/container/string.hpp>
#include <boost/container/scoped_allocator.hpp>
#include <boost/foreach.hpp>

// map에서 해제하지 않는 것을 전제로 스택에서 해제 무시의 간단한 할당기
template<unsigned N>
class SampleAlloc3 {
public:
    SampleAlloc3() : size_(0) { }
    void* allocate(unsigned n) {
        assert(size_+n <= N);
        void* p = ptr(size_);
        size_ += n;
        return p;
    }
    void  deallocate(void*, unsigned) { /*dummy*/ }
private:
    typedef double buf_t;
    void*       ptr(unsigned n=0) { return (char*)buf_ + ((n + sizeof(buf_t)-1) & ~(sizeof(buf_t)-1)); }
private:
    unsigned    size_;
    buf_t       buf_[N / sizeof(buf_t)];
};

// map<string,int> 의 메모리 
template<unsigned SAMPLE_ALLOC3_SIZE>
class Hoge {
public:
    Hoge() : strIntMap_(std::less<String>(), MapAllocator(StrIntPairAlloc(&smpAlc3_))) {}
    void insert(const char* name, int val) {
        //strIntMap_[String(name, &smpAlc3_)] = val;
        strIntMap_.emplace( name, val );
    }
    void printAll() {
        BOOST_FOREACH(StrIntPair p , strIntMap_)
            printf("%s = %d\n", p.first.c_str(), p.second);
    }
private:
    typedef SampleAlloc3<SAMPLE_ALLOC3_SIZE> Alloc;
    template<typename T>
    class SampleAllocator3 {
    public:
        typedef T value_type;
        // SampleAllocator3() : alc_(0) {}  // default 생성자는 준비하지 않는 것이 안전
        SampleAllocator3(Alloc* alc) : alc_(alc) { }
        template<typename U> SampleAllocator3(SampleAllocator3<U> const& r) : alc_(r.detail_get_ptr()) { }
        T* allocate(unsigned n) { return reinterpret_cast<T*>(alc_->allocate(n*sizeof(T))); }
        void  deallocate(T* p, unsigned n) { alc_->deallocate(p,n*sizeof(T)); }
        bool operator == (SampleAllocator3 const & ) const { return false; }
        bool operator != (SampleAllocator3 const & ) const { return true; }
        Alloc* detail_get_ptr() const { return alc_; }
    private:
        Alloc*      alc_;
    };
    typedef boost::container::basic_string<char, std::char_traits<char>, SampleAllocator3<char> >   String;
    typedef std::pair<const String, int>    StrIntPair;
    typedef SampleAllocator3<StrIntPair>    StrIntPairAlloc;
    typedef boost::container::scoped_allocator_adaptor< StrIntPairAlloc >           MapAllocator;
    typedef boost::container::map<String, int, std::less<String>, MapAllocator >    StrIntMap;
private:
    SampleAlloc3<SAMPLE_ALLOC3_SIZE>    smpAlc3_;
    StrIntMap                           strIntMap_;
};

void sample3()
{
    Hoge<1024>  hoge;
    hoge.insert("xyz", 1);
    hoge.insert("abcdefghijklmnopqrstuvwxyz", 2);
    hoge.insert("1234567890", 3);
    hoge.printAll();
}
```
  
 