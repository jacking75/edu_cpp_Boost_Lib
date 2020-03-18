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
```
  
template <class T, class Allocator>
class vector : vector_alloc_holder <Allocator>;
(SGI 버전 파생 등) 다른 구현이라고 모든 포인터에서 관리하는 것이 많습니다 만,이 구현은 size, capacity를 수로가 있습니다 (개인적으로 디버깅 할 때 멤버 변수에서 볼 수 있으므로 취향) .

Allocator는 (비 static) 멤버 변수가 없으면 진짜 0 바이트가되도록 struct의 상속되어 있습니다.
stl의 구현이 약간의 차위는있는 Allocator는 (비 static) 멤버 변수가있는 것이 많습니다 만, C ++ 03의 경우 static 멤버 변수로 갖는 것도 있음이었다 같고, 실제로 그렇게되지 컴파일러도있었습니다.
C ++ 11에서는 scoped_allocator_adaptor의 수도 있고 Allocator는 (비 static) 멤버 변수 필수 인 것 같습니다.

것으로 C ++ 11면

// 1 회 N 개의 아로케토 그냥 Allocator
template <typename T, unsigned N>
class SampleAllocator1 {
public :
   typedef T value_type;
   SampleAllocator1 () {}
   T * allocate (unsigned n) {assert (n == N); return reinterpret_cast <T *> (buf_);}
   void deallocate (T *, unsigned) {}
   bool operator == (SampleAllocator1 const &) const {return false;}
   bool operator! = (SampleAllocator1 const &) const {return true;}
private :
   typedef double buf_t;
   buf_t buf _ [(N * sizeof (T) + sizeof (buf_t) -1) / sizeof (buf_t);
};
// typedef std :: vector <int, SampleAllocator1 <int, 1024>> Int1024Vector;
typedef boost :: container :: vector <int, SampleAllocator1 <int, 1024>> Int1024Vector;
Int1024Vector g_intVec;
같은 흉내를 내도 괜찮을 것 ... 그리고 썼지 만 위는 아마 boost :: container에 의존하고 있습니다.
(대상 환경에서 malloc 준비 전에 (고정 크기) 컨테이너를 사용하고 .. . 적도 있었다)

↑
list
동결
구현은 boost :: intrusive의 정의도 많은 귀찮아서 꽤 端折っ하고 (boost :: container로 아니라) 있음직 한 list 구현 회원의 분위기로 다음 (죄송 후문에서 boost :: container 묻힌 좋은 그렇지만 대처하지 못하고 ...)

class Node_Base {
    Node_Base * next_;
    Node_Base * prev_;
};
template <typename T>
class Node : Node_Base {
    T value_;
};
template <typename T, class A>
class List : A {
    ListNode_Base root_;
    size_t size_;
};
꽤 적당하지만 메모리의 분위기가 알면 ...
↑
map, multimap, set multiset
동결
map, set은 대부분 레드 - 블랙 트리 구현 같고, boost :: container는 boost :: intrusive을 사용하고있는 것 같습니다.
그리고, 이것도 메모리의 분위기 만 ...

class Node_Base {
    Node_Base * left_;
    Node_Base * right_;
    Node_Base * parent_;
    bool balance_;
};
template <typename T>
class Node : Node_Base {
    T value_;
};
template <typename T>
class Tree : A {
    Node_Base root_;
    size_t size_;
};

List와 Map은 전달 된 할당을 그대로 사용하는 것이 아니라, allocator의 회원
template <class U> struct rebind {typedef allocator <U> other;};
를 사용하여 T 아니라 Node <T> allocator를 사용하고 있습니다. 또한 allocator에 대한 요청은 일반적으로 1 개 단위가된다고 생각합니다.

것으로, list 나 map의 나는 나 할당은 노드 크기 고정 메모리를 풀 두는라는 것이있을 수 있습니다.

template <unsigned B, unsigned N>
class SampleAlloc2 {
public :
    SampleAlloc2 () {
        for (unsigned i = 0; i <N-1; ++ i)
            buf_ [i] .link = & buf_ [i + 1];
        buf_ [N-1] .link = NULL;
        root_ = & buf_ [0];
    }
    void * allocate (unsigned n) {
        assert (n == 1 && root_);
        Link * p = root_;
        root_ = p-> link;
        return p;
    }
    void deallocate (void * t, unsigned n) {
        if (t) {
            assert (n == 1);
            Link * p = reinterpret_cast <Link *> (t);
            p-> link = root_;
            root_ = p;
        }
    }
private :
    union Link {
        Link * link;
        char b [B];
    };
private :
    Link * root_;
    Link buf_ [N];
};
enum {ELM_SIZE = 32}; // 컨테이너 노드를 포함하는 요소의 바이트 수.
enum {MAX_SIZE = 16};
template <typename T>
class SampleAllocator2 : public SampleAlloc2 <ELM_SIZE, MAX_SIZE> {
    typedef SampleAlloc2 <ELM_SIZE, MAX_SIZE> base_type;
public :
    typedef T value_type;
    SampleAllocator2 () {BOOST_STATIC_ASSERT (sizeof (T) <= ELM_SIZE);}
    T * allocate (unsigned n) {return reinterpret_cast <T *> (base_type :: allocate (n));}
    void deallocate (T * t, unsigned n) {base_type :: deallocate (t, n);}
    bool operator == (SampleAllocator2 const &) const {return false;}
    bool operator! = (SampleAllocator2 const &) const {return true;}
};
// list
typedef SampleAllocator2 <int> IntAllocator;
typedef boost :: container :: list <int, IntAllocator> IntList;
// map
struct cstr_less {
    bool operator () (const char * l, const char * r) const {return strcmp (l, r) <0;}
};
typedef std :: pair <char const * const, int> smpPair_t;
typedef SampleAllocator2 <smpPair_t> SmpPairAllocator;
typedef boost :: container :: map <const char *, int, cstr_less, SmpPairAllocator> StrIntMap;
※ 요소 (노드) 크기 (ELM_SIZE)이 직접 집에서 품위입니다 만, 우선은.

↑
deque
동결
(구현 번거 로움과 대상에서 그다지 사용하지 않고 ...하지만 마감 시간. 나중에 뭔가)

↑
string
동결
std :: string은 (C ++ 03에서) 라이브러리에 대해 상당히 구현이 바라케 있습니다 (EffectiveSTL에 실리고 라든지). c ++ 11에서 속박이 바짝 되었기 때문에 다소으로 다니고 올지도 만, 그래도 여러가지 할 수있을 것입니다.

최소한으로 vector와 같은 내용이 될 것입니다 ... boost :: container :: string에서 端折っ을 뽑아 보면

 struct long_t {
    size_type is_short : 1;
    size_type length : (sizeof (size_type) * CHAR_BIT - 1);
    size_type storage;
    pointer start;
 };
 struct short_header {
    unsigned char is_short : 1;
    unsigned char length : (CHAR_BIT - 1);
 };
 struct short_t {
    short_header h;
    value_type data [UnalignedFinalInternalBufferChars]; 
                   // UnalignedFinalInternalBufferChars ≒ (sizeof (long_t) -sizeof (short_header)) / sizeof (value_type)
 };
 union repr_t {
    long_raw_t r;
    short_t s;
 };
long_t 긴 문자열 할당을 사용하는 경우, sizeof (long_t) -α 이내에 가라 앉는다 짧은 경우 short_t과 같이 영역을 버퍼로 사용 동적 확보하지 않고 끝내야한다는 궁리가되고 있습니다. 64bit CPU라고 vector 호환도 sizeof (포인터) * 3 = 24bytes합니다, 상당히 고마운 구현입니다. (최근 만들고 있었던 나는 내가 string이 바로이 유형이었다 ... 또 boost 때문에 자주)

※ 참고 : 비트 필드는 엔디 관계없이 먼저 쓰여진 것이 낮은 주소에 배치된다.

↑
stable_vector, flat_map, flat_set
동결
stl 컨테이너의 대용품이 아니라 vector 구현을 기반으로 한 특화 버전.
stable_vector는 vector <T *>와 요소를 별도 아로케토 한 것. (boost :: ptr_vector의 유사품?)
flat_map, flat_set는 vector <T> (map은 T = pair <KEY, VALUE>)에 정렬 상태로 등록하여 인터페이스가 map과 set가 된 것.
각자 나는 나 컨테이너를 만들어 것 같은 종류이므로 (그래 만들고있었습니다), boost에 있으면 편하게 될 것입니다. (있다면 stable_vector을 flat_map 변한 것도 있고 즐거운 곳)

자세한 것은 다른 사이트에 임하고 있습니다.

↑
scoped_allocator_adaptor
동결
C ++ 11에서 늘어난 할당합니다.
scoped_allocator_adaptor을 이용하면, 예를 들면 map <string, int> 같은 컨테이너에서 로컬 할당을 string과 map에서 공통으로 사용할 수 있도록 할 수 있습니다.
※ 내부 컨테이너 (이 경우 string) 생성자로 기본 생성자가 아닌 할당을 인수에 취하는 생성자가 사용되게됩니다.

할당을 컨테이너의 멤버로하기 때문에, 당연히 메모리 사용량도 증가 (특히 안쪽의 컨테이너는 수가 많아 쉽게). 때문에 컨테이너의 구성원 할당은 포인터 만하고 구현뿐만 아니라 그 준비, 같은 느낌으로하게 될 것입니다.

자세한 것은 이쪽 의 사이트 라든지 보여달라고하는 편이 좋습니다.

솔직히보기에 잘 몰라서 시도 어쩐지 납득 상태입니다.

이하, 시도 소스.

#include <stdio.h>
#include <boost / container / map.hpp>
#include <boost / container / string.hpp>
#include <boost / container / scoped_allocator.hpp>
#include <boost / foreach.hpp>

// map에서 해제하지 않는 것이 전제 스택에서 해방 무시 간이 할당.
template <unsigned N>
class SampleAlloc3 {
public :
    SampleAlloc3 () : size_ (0) {}
    void * allocate (unsigned n) {
        assert (size_ + n <= N);
        void * p = ptr (size_);
        size_ + = n;
        return p;
    }
    void deallocate (void *, unsigned) {/ * dummy * /}
private :
    typedef double buf_t;
    void * ptr (unsigned n = 0) {return (char *) buf_ + ((n + sizeof (buf_t) -1) & ~ (sizeof (buf_t) -1));}
private :
    unsigned size_;
    buf_t buf_ [N / sizeof (buf_t);
};

// map <string, int> 메모리
template <unsigned SAMPLE_ALLOC3_SIZE>
class Hoge {
public :
    Hoge () : strIntMap_ (std :: less <String> () MapAllocator (StrIntPairAlloc (& smpAlc3_))) {}
    void insert (const char * name, int val) {
        // strIntMap_ [String (name, & smpAlc3_)] = val;
        strIntMap_.emplace (name, val);
    }
    void printAll () {
        BOOST_FOREACH (StrIntPair p, strIntMap_)
            printf ( "% s = % d \ n", p.first.c_str () p.second);
    }
private :
    typedef SampleAlloc3 <SAMPLE_ALLOC3_SIZE> Alloc;
    template <typename T>
    class SampleAllocator3 {
    public :
        typedef T value_type;
        // SampleAllocator3 () : alc_ (0) {} // 기본 생성자는 제공하지 않는 편이 안전합니다.
        SampleAllocator3 (Alloc * alc) : alc_ (alc) {}
        template <typename U> SampleAllocator3 (SampleAllocator3 <U> const & r) : alc_ (r.detail_get_ptr ()) {}
        T * allocate (unsigned n) {return reinterpret_cast <T *> (alc _> allocate (n * sizeof (T)));}
        void deallocate (T * p, unsigned n) {alc _> deallocate (p, n * sizeof (T));}
        bool operator == (SampleAllocator3 const &) const {return false;}
        bool operator! = (SampleAllocator3 const &) const {return true;}
        Alloc * detail_get_ptr () const {return alc_;}
    private :
        Alloc * alc_;
    };
    typedef boost :: container :: basic_string <char std :: char_traits <char> SampleAllocator3 <char>> String;
    typedef std :: pair <const String, int> StrIntPair;
    typedef SampleAllocator3 <StrIntPair> StrIntPairAlloc;
    typedef boost :: container :: scoped_allocator_adaptor <StrIntPairAlloc> MapAllocator;
    typedef boost :: container :: map <String, int std :: less <String> MapAllocator> StrIntMap;
private :
    SampleAlloc3 <SAMPLE_ALLOC3_SIZE> smpAlc3_;
    StrIntMap strIntMap_;
};

void sample3 ()
{
    Hoge <1024> hoge;
    hoge.insert ( "xyz", 1);
    hoge.insert ( "abcdefghijklmnopqrstuvwxyz", 2);
    hoge.insert ( "1234567890", 3);
    hoge.printAll ();
}
emplace, emplace_back 단지 (복사) 생성자의 빈도 낮게뿐만 아니라, 이번 같은 경우 바로 필수 종류였습니다.
공통으로 사용되는 할당 (SampleAllocator3) 디폴트 생성자를 붙여 안돼,라는 것을 실감.
(기본 생성자 붙이고있는 경우 insert 나 push_back 사용해도 컴파일 통과하기 ... 빠졌습니다)  