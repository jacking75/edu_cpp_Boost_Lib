# Boost.Geometry 
  
## 뛰어난 설계로된 Boost.Geometry
출처: http://mmiyano.blogspot.com/2012/02/boostgeometry.html     

최근 Boost.Geometry의 2D boolean을 사용해 보면 성능과 견고성에 놀랐다.  
또한 Boost.Geometry 디자인이 훌륭하다.   
알기 쉬운 소재를 바탕으로 traits, 태그 디스패치, 개념, 메타 함수 등의 템플릿 주위의 독특한 기법이 조금씩 등장하므로,이 개요를 파악 하기에도 아주 좋은 자료라고 생각한다.  
  
원문은 [여기](http://www.boost.org/doc/libs/1_48_0/libs/geometry/doc/html/geometry/design.html)    
  
  
### 설계의 이론적 기초(Design Rationale)
만일 두 점 사이의 거리를 계산하는 C++ 프로그램을 당신이 필요하게 되었다고 하자. 당신은 구조체를 정의 할것이다.  
```
struct mypoint
{
    double x, y;
};
```
  
그리고 알고리즘을 설명하는 함수.  
```
double distance(mypoint const& a, mypoint const& b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}
```
  
매우 간단하고 사용도 충분하다. 그러나 제네릭이 아니다. 라이브러리로 좀 더 디자인을 개선해야 한다. 위의 디자인에서 사용할 수있는 것은 2 차원의 점, mypoint 라는 구조체, 게다가 직교 좌표계에 한해서이다. 제네릭 라이브러리는 아래와 같은 거리의 계산이 가능해야한다:  
- mypoint 구조체뿐만 아니라 모든 점 클래스 구조체에 대해
- 2차원 이상의 공간에서도
- 지구나 구면 같은 다른 좌표계에서도
- 점과 선, 기타 지오메트리 사이에서
- double 보다 높은 정밀도로
- 제곱근의 계산을 피하면서(제곱근의 계산은 상대적으로 높은 비용 함수로 거리의 비교에서는 필요가 없기 때문에 사용하고 싶지 않다)
    
앞으로 계속되는 섹션에서는 단계별로 더욱 제네릭한 설계를 해 나간다.  
  

### 템플릿 이용(Using Templates)
distance 함수는 템플릿 함수로 변경할 수 있다. 이것은 간단하고 mypoint 타입 이외의 다른 포인트 타입에 대해서도 거리를 계산할 수 있게 된다. 다른 2개의 포인트 타입도 허용하기 위해 2개의 템플릿 인수를 추가한다.  
```
template <typename P1, typename P2>
double distance(P1 const& a, P2 const& b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}
```
  
이 템플릿 버전은 약간은 좋다. 그러나 충분하지 않다.  
멤버 변수 x나 y에 직접 접근하는 대신에 C++ 클래스를 생각해 보자. 따라서 이 절은 빨리 끝내고 다음 나아자.  
  

### Traits 이용(Using Traits)
우리는 일반적인 접근 방식을 취하고 distance 함수의 입력에 다양한 포인트 타입을 허용해야 한다. 멤버 변수 x나 y에 접근하는 대신 traits 시스템을 이용하여 몇 단계의 간접 참조를 추가한다. 함수는 다음과 같다:  
```
template <typename P1, typename P2>
double distance(P1 const& a, P2 const& b)
{
    double dx = get<0>(a) - get<0>(b);
    double dy = get<1>(a) - get<1>(b);
    return std::sqrt(dx * dx + dy * dy);
}
```
  
이 개조된 distance 함수는 점의 좌표에 접근 하기 위해 차원을 템플릿 인수로 받는 제네릭 get 함수를 호출하고 있다. 이 get 함수는 아래와 같이 정의된 traits 시스템으로 향한다:  
```
namespace traits
{
    template <typename P, int D>
    struct access {};
}
```
  
아래는 우리의 mypoint 타입에 특수화된 것으로 get은 static 함수로 구현되어 있다:  
```
namespace traits
{
    template <>
    struct access<mypoint, 0>
    {
        static double get(mypoint const& p)
        {
            return p.x;
        }
    };
    // same for 1: p.y
    ...
}
```
  
`traits::access<mypoint, 0>::get(a)`를 호출하는 것으로 우리의 x 좌표가 반환된다. 이 함수의 호출 방법은 자주 라이브러리에 사용되는 것으로는 너무 장황하다. 우리는 아래의 짧은 함수(free extra function)를 추가 할 수 있다:  
```
template <int D, typename P>
inline double get(P const& p)
{
    return traits::access<P, D>::get(p);
}
```
  
이것에 의해 이 절의 첫 번째 distance 알고리즘에서 설명된대로 `traits::access`가 특수화된 모든 포인트 타입에 대해서 `get<0>(a)`을 호출 할 수 있다. 따라서 x() 같은 메소드를 가진 클래스에 대해서 사용하고 싶다면 차원 0에 대해서 x()를, 차원 1에 대해서 y()를 반환하도록 스태틱한 get 함수를 통해서 구조체에 접근하도록 특수화가 있으면 된다.  
  

### 차원의 독립성(Dimension Agnosticism)
지금 우리는 모든 구조체와 클래스의 2차원 점 사이의 거리를 계산 가능해졌다. 그러나 우리는 3차원도 다루고 싶다. 그래서 차원에 대해 독립적으로 할 필요가 있다. 이것은 우리의 distance 함수를 까다롭게 한다. 우리는 for 루프를 사용하여 차원을 걷어서 통과 할 수 있다. 하지만 for 루프는 추가 성능이 필요하다. 우리는 새로운 템플릿을 이용해서 더 복잡하지만 템플릿 매니아들을 매료시키는 distance 알고리즘을 만들 수 있다:  

```
template <typename P1, typename P2, int D>
struct pythagoras
{
    static double apply(P1 const& a, P2 const& b)
    {
        double d = get<D-1>(a) - get<D-1>(b);
        return d * d + pythagoras<P1, P2, D-1>::apply(a, b);
    }
};

template <typename P1, typename P2 >
struct pythagoras<P1, P2, 0>
{
    static double apply(P1 const&, P2 const&)
    {
        return 0;
    }
};
```
  
distance 함수는 차원 수를 지정하여 pythagoras 구조체를 호출:  
```
template <typename P1, typename P2>
double distance(P1 const& a, P2 const& b)
{
    BOOST_STATIC_ASSERT(( dimension<P1>::value == dimension<P2>::value ));

    return sqrt(pythagoras<P1, P2, dimension<P1>::value>::apply(a, b));
}
```
  
차원은 아래와 같이 정의된 다른 traits 클래스를 사용하여 참조된다:  
```
namespace traits
{
    template <typename P>
    struct dimension {};
}
```
  
이것은 mypoint 구조체로 전문화 될 필요가 있다.  
값을 발행할 뿐이므로, `Boost.MPL의 class boost::mpl::int_` 를 파생 시키면 편리하다.:   
```
namespace traits
{
    template <>
    struct dimension<mypoint> : boost::mpl::int_<2>
    {};
}
```
  
위의 get 함수처럼, 라이브러리는 dimension 메타 함수도 가진다.  
```
template <typename  P> 
struct dimension : traits::dimension<P> 
{};
```
   
왜 이 선언이 유용한지 설명하겠다. 지금 우리는 차원에 대해 독립적으로 되었다. distance 함수는 3차원 또는 그 이상의 차원의 점을 취급 할 수 있게 되었지만, 2차원의 점 a와 3 차원 점b에 대한 연산을 컴파일 시의 주장에 의해 방지 할 수 있다.  
  

### 좌표 값 타입(Coordinate Type)
지금까지 우리는 좌표 값은 double 타입으로 결정하고 달려왔다. 만약 우리의 점이 integer 타입이라면 어떻게?  
  
traits 클래스를 쉽게 추가 할 수 있으며, 실제로 그렇게 한다. 그러나 2개의 정수 좌표의 거리는 double 그대로 할 수 있다. 또한 이와는 별도로 하여 디자인 목표는 제곱근을 피하기 위한 것이었다. 계속 다른 절에서는 이러한 경우를 다룬다. 그러나 지금은 2개의 정수 좌표를 취급하지만 결과는 double로 반환하자. 좌표 값 타입을 정의하려면 또 하나의 traits 클래스 coordinate_type를 추가한다. 이것은 라이브러리의 사용자에 의해 아래와 같이 특수화 되어야한다:  
```
namespace traits
{
    template <typename P>
    struct coordinate_type{};

    // specialization for our mypoint
    template <>
    struct coordinate_type<mypoint>
    {
        typedef double type;
    };
}
```  
<<<<<<<아래 부분 계속 번역해야 한다>>>>>>>  
access 함수 get 무료 함수를 정의했듯이 프록시를 같이 추가한다. 더 긴 버전은 나중에 등장하지만 짧은 버전은 다음과 같다 :

template  < typename  P > 
struct  coordinate_type  :     traits :: coordinate_type < P >  {};
지금 다시 distance 함수가 수정할 수있다. 여전히 double 형으로 결과를 반환하기 때문에, 우리는 pythagoras클래스 만 수정하면된다. 결과는 입력과 같은 좌표 값 형식을 반환한다. 그러나 인풋은 두 가지 유형을받을 수있다. 그들은 다른 좌표 값 형지도 모른다. 이것은 드문 일이지만 우리는 제네릭 라이브러리를 디자인하고 이러한 특이한 케이스도 취급한다. 우리는 둘 중 정밀도가 높은 쪽을 좌표 값 유형으로 선택할 필요가있다. 이것에 대해서는 여기서 설명하지 않는다. 너무 길어질 수와 기하학과는 관계가 없기 때문이다. 지금은 적절한 형식을 선택 메타 함수로 select_most_precise가있는 것으로 가정하자.

우리의 계산 클래스는 다음과 같이 보입니다 :

template  < typename  P1 ,  typename  P2 ,  int  D > 
struct  pythagoras 
{ 
    typedef  typename  select_most_precise 
        < 
            typename  coordinate_type < P1 > :: type , 
            typename  coordinate_type < P2 > :: type 
        > :: type  computation_type ;

    static  computation_type  apply ( P1  const &  a ,  P2  const &  b ) 
    { 
        computation_type  d  =  get < D - 1 > ( a )  -  get < D - 1 > ( b ); 
        return  d  *  d  +  pythagoras  < P1 ,  P2 ,  D - 1 >  :: apply ( a , b ); 
    } 
};
다른 기하학적 유형 (Different Geometries)
모든 형태와 좌표의 점을 지원하는 차원에서 독립적 인 시스템을 설계 할 수 있었다. 아직 몇 가지 조정이 있지만 나중에 설명하자. 지금부터 어떻게하여 점과 다각형이나 점과 선 사이의 거리를 계산하는 방법을 살펴 보자. 이러한 공식은 더 복잡 디자인에 미치는 영향도 더 큰 것으로된다. 우리는 다음과 같은 다른 이름을 가진 함수를 추가 싶지는 않다 :

template  < typename  P ,  typename  S > 
double  distance_point_segment ( P  const &  p ,  S  const &  s )
우리는 제네릭하고 싶다. distance 함수는 처리 기하 유형을 알 수 없음으로 불려 갈 필요가있다. 그래서 이름은 distance이어야한다. 우리는 또한 오버로드 할 수 없다. 왜냐하면 동일한 템플릿 시그니처가있는 경우 애매 해져 버리기 때문이다. 여기에는 두 가지 해결책이있다 :

tag dispatching (태그 디스패치)
SFINAE
우리는 태그 디스패치를 ​​선택했다. traits 시스템에 잘 맞는했기 때문이다. 초기 버전의 Boost.Geometry는 SFINAE을 사용했지만 이런 큰 디자인에 대한 몇 가지 단점이 발견 되었기 때문에, 태그 디스패치로 변경되었다.

태그 디스패치 의해 distance 알고리즘은 입력 매개 변수의 기하 유형을 알아낼 수있다. distance 함수는 다음과 같이 변경된다 :

template  < typename  G1 ,  typename  G2 > 
double  distance ( G1  const &  g1 ,  G2  const &  g2 ) 
{ 
    return  dispatch :: distance 
        < 
            typename  tag < G1 > :: type , 
            typename  tag < G2 > :: type , 
            G1 ,  G2 
        > :: apply ( g1 ,  g2 ); 
}
tag 메타 함수를 참조하고 dispatch :: distance 의 apply 메소드를 호출한다. tag 메타 함수는 또 다른 traits 클래스이며, 포인트 형마다 전문화된다. 그들은이있게된다 :

namespace  traits 
{ 
    template  < typename  G > 
    struct  tag  {};

    // specialization 
    template  <> 
    struct  tag < mypoint > 
    { 
        typedef  point_tag  type ; 
    }; 
}
coordinate_system get와 같은 후리메타 기능 :

template  < typename  G > 
struct  tag  :  traits :: tag < G >  {};
Tags (point_tag, segment_tag, etc)는 dispatch 구조를 특수화하는 목적의 빈 구조체이다. distance 대한 dispatch 구조체과 전문화는 다음과 같은 고립 된 네임 스페이스에 모두 정의된다 :

namespace  dispatch  { 
    template  <  typename  Tag1 ,  typename  Tag2 ,  typename  G1 ,  typename  G2  > 
    struct  distance 
    {};

    template  < typename  P1 ,  typename  P2 > 
    struct  distance  <  point_tag ,  point_tag ,  P1 ,  P2  > 
    { 
        static  double  apply ( P1  const &  a ,  P2  const &  b ) 
        { 
            // 여기에 이미 온 같은 
            // pythagoras를 호출 
            . .. 
        } 
    };

    template  < typename  P ,  typename  S > 
    struct  distance 
    < 
        point_tag ,  segment_tag ,  P ,  S 
    > 
    { 
        static  double  apply ( P  const &  p ,  S  const &  s ) 
        { 
            // 여기에 2,3 차원에서 
            // 점과 선 분의 거리를 계산 
            // 다른 함수를 참조하는 
            ... 
        } 
    };

    // 여기에는 다음과 같은 새로운 전문화가 
    //일지도 모른다 
    // 점 - 다각형 상자 - 원형, etc.

}  // namespace
이제 distance 알고리즘은 제네릭되고 다른 기하 타입도 지원할 수있게되었다. 하나의 단점은 점 - 선, 선 - 점의 2 개의 전문화를해야 할 것이다. 이 후 절에서 해결된다. 다음은 다른 포인트 형 기하 유형 차원에 대한 샘플이다 :

point  a ( 1 , 1 ); 
point  b ( 2 , 2 ); 
std :: cout  <<  distance ( a , b )  <<  std :: endl ; 
segment  s1 ( 0 , 0 , 5 , 3 ); 
std :: cout  <<  distance ( a ,  s1 )  <<  std :: endl ; 
rgb red ( 255 ,  0 ,  0 ); 
rbc  orange ( 255 ,  128 ,  0 ); 
std :: cout  <<  "color distance :"  <<  distance ( red ,  orange )  <<  std :: endl ;
핵심 부분에 다시 돌아 가기 (Kernel Revisited)
traits 네임 스페이스에서 각각 정의 된 traits 클래스 coordinate_type를 사용하여왔다. 이것은 실제로는 필요하지 않다. 왜냐하면 유일한 차이점은 이름 공간이기 때문이다. 그러나 이제 우리는 다른 기하학적 형태를 갖게되었다. 이것은 근본적인 문제이다. 우리는 모든 형상 유형, 점, 선, 다각형 등을 지원하는 태그 디스패치를 이용하여 구현 된 coordinate_type메타 함수를 호출 할 수 있습니다 :

template  < typename  G > 
struct  coordinate_type 
{ 
    typedef  typename  dispatch :: coordinate_type 
        < 
            typename  tag < G > :: type ,  G 
        > :: type  type ; 
};
dispatch 네임 스페이스 내에서이 메타 함수는 두 번 구현된다. 일반 버전과 점에 특수화 된 것이다. 점에 특수화 된 것은 traits 클래스를 호출한다. 제네릭 버전은 점의 특수화 된 것을 호출한다. 재귀 메타 함수 정의 같은 것이다 :

namespace  dispatch 
{

    // Version for any geometry : 
    template  < typename  GeometryTag ,  typename  G > 
    struct  coordinate_type 
    { 
        typedef  typename  point_type 
            < 
                GeometryTag ,  G 
            > :: type  point_type ;

        // Call specialization on point-tag 
        typedef  typename  coordinate_type  <  point_tag ,  point_type  > :: type  type ; 
    };

    // Specialization for point-type : 
    template  < typename  P > 
    struct  coordinate_type < point_tag ,  P > 
    { 
        typedef  typename 
            traits :: coordinate_type < P > :: type 
            type ; 
    }; 
}
그런데, 다른 메타 함수 point_type를 호출한다. 여기에서는 자세히 설명하지 않지만, 이것이 모든 기하 유형 기하를 형성하는 점의 typedef에 호출을 가능하게하고있다.

이와 같은 것을 메타 함수 dimension과 이어 등장하는 메타 함수 coordinate sysytem에 적용한다.

좌표계 (Coordinate System)
여기까지는 직교 좌표계 가정왔다. 그러나 우리는 지구가 평면이 아닌 것을 알고있다. 직교 좌표계에서 2 개의 GPS 좌표의 거리를 계산하는 것은 무의미하다. 따라서 다시 설계를 확장한다. traits 시스템을 다시 사용하는 것으로, 각각의 포인트 형에 대해서 좌표계를 정의한다. 다음 좌표계에 맞는 계산을 만듭니다.

좌표계는 좌표 값 형과 비슷하다. 메타 함수에서 dispath 함수를 가진 모든 기하 유형을 호출, 그 점의 전문화에 가서 결국에는 traits 클래스를 호출 좌표계를 가진 형태를 정의한다. 여기에서는 그 모든 것은 보이지 않는다. 정의의 일부는 다음과 같다 :

struct  cartesian  {};

template < typename  DegreeOrRadian > 
struct  geographic 
{ 
    typedef  DegreeOrRadian  units ; 
};
직교 좌표계 (cartesian)은 단순하다. 지리 좌표계 (geographic)는도 (degree) 또는 라디안 (radian)의인지를 선택할 수있다.

distance 함수는 좌표계에 대응 한 계산 방법을 선택하고 그 때 distance 대한 dispath 구조체를 호출하도록 변경된다. 우리는 좌표계에 특수화 된 계산 방법을 전략 (strategy)이라고 부른다. 새로운 버전의 distance 함수는 다음과 같이 보입니다 :

template  < typename  G1 ,  typename  G2 > 
double  distance ( G1  const &  g1 ,  G2  const &  g2 ) 
{ 
    typedef  typename  strategy_distance 
        < 
            typename  coordinate_system < G1 > :: type , 
            typename  coordinate_system < G2 > :: type , 
            typename  point_type < G1 > : : type , 
            typename  point_type< G2 > :: type , 
            dimension < G1 > :: value 
        > :: type  strategy ;

    return  dispatch :: distance 
        < 
            typename  tag < G1 > :: type , 
            typename  tag < G2 > :: type , 
            G1 ,  G2 ,  strategy 
        > :: apply ( g1 ,  g2 ,  strategy ()); 
}
strategy_distance 다른 좌표계에 대한 전문화를 가진 구조이다.

template  < typename  T1 ,  typename  T2 ,  typename  P1 ,  typename  P2 ,  int  D > 
struct  strategy_distance 
{ 
    typedef  void  type ; 
};

template  < typename  P1 ,  typename  P2 ,  int  D > 
struct  strategy_distance < cartesian ,  cartesian ,  P1 ,  P2 ,  D > 
{ 
    typedef  pythagoras < P1 ,  P2 ,  D >  type ; 
};
다시 pythagoras등장하고, 여기에 전략으로 정의되어있다. distance는 apply 메소드를 호출하는 함수를 dispatch한다.

그리고 이것이 중요한 단계가된다. 구면 및 지리 좌표계에 대해 다른 전략을 구현하는 것이 가능하게된다. 구면 좌표계는 haversine 공식이있다. 따라서 발송하는 traits 구조는 이렇게 전문화 된

template  < typename  P1 ,  typename  P2 ,  int  D  =  2 > 
struct  strategy_distance < spherical ,  spherical ,  P1 ,  P2 ,  D > 
{ 
    typedef  haversine < P1 ,  P2 >  type ; 
};

// struct haversine with apply function 
// is omitted here
지리 좌표계에서 우리는 몇 가지 다른 거리 계산 방법이있다. Andoyer 법은 빠르고 정확하다. 그리고 Vincenty 법은 느리지 만 더 정확하다. 또한 그 밖에도 정확도 떨어지는 방법들이 존재한다.

좌표계에 대해 기본 전략이 하나 정의되어있다. 다른 전략도 마찬가지로 사용할 수 있도록하기 위해, 우리는 디자인을 다시 수정하고 distance 알고리즘이 제 3 인수로 전략을 받도록 오버로드를 추가한다.

이 새로운 오버로드 distance 함수는 전략이 distance 함수의 외부에서 구축 할 수 있다는 장점도 가진다. 왜냐하면 만약 내부에 구축되면, 구축시의 매개 변수를 가질 수 없기 때문이다. Andoyer 법이나 Vincenty 법, haversine 공식은 지구의 반경을 매개 변수로받는 생성자를함으로써 의미를 가진다.

오버로드 된 distance 함수는 다음과 같이 보입니다 :

template  < typename  G1 ,  typename  G2 ,  typename  S > 
double  distance ( G1  const &  g1 ,  G2  const &  g2 ,  S  const &  strategy ) 
{ 
    return  dispatch :: distance 
        < 
            typename  tag < G1 > :: type , 
            typename  tag < G2 > :: type , 
            G1 ,  G2 , S 
        > :: apply ( g1 ,  g2 ,  strategy ); 
}
전략은 두 가지 점을 인자로받는 메소드를 가질 필요가있다. 이것은 정적 메소드 인 필요는 없다. 전략은 변수를 받아 멤버 변수에 저장하는 생성자를 정의 할지도 모른다. 이런 경우에는 정적 메소드에서는 불편 것이다. 이것은 정상적인 방법 (const 성을 가진)로 구현할 수있다.

여기에서는 모든 구현을 열거하지 않는다. Vincenty 법은 대부분의 페이지는 수학에 가득차있다. 그러나 아이디어는 이해할 수있을 것이다. 우리는 distance를 다음과 같이 호출 할 수 있습니다 :

distance ( c1 ,  c2 )
여기서 c1와 c2는 직교 좌표계에서 점이다. 또는 :

distance ( g1 ,  g2 )
여기서 g1와 g2는 지리 좌표계의 점이며 기본 전략 (예 Andoyer)를 호출한다. 또는 :

distance ( g1 ,  g2 ,  vincenty < G1 ,  G2 > ( 6275 ))
여기에 전략은 명시 적으로 지정된 반경 값으로 구축되어있다.

포인트 개념 (Point Concept)
앞 절에서 언급 한 5 개의 traits 클래스는 포인트 개념 (Point Concept)을 형성한다. traits 네임 스페이스에서 전문화가 구현 된 모든 포인트 형식은 유효한 형태로 받아 들일 것이다. Point Concept는 다음 구성하고있다 :

traits::tag전문화
traits::coordinate_system전문화
traits::coordinate_type전문화
traits::dimension전문화
traits::access전문화
마지막 하나는 get 메소드와 (옵션) set 메소드를 가진 클래스이다. 이전 4 개는 형식 정의와 값의 선언을하는 메타 함수이다 (MPL에 따라).

그런데, 우리는 이제 차원의 수에 비 의존성 좌표계에 독립성을 얻었다. 이 디자인은 모든 좌표 값 형 다른 기하 유형을 처리 할 수있다. 심지어 자신의 전략을 지정할 수 두 점의 차원이 다를 경우 (주장에 의해) 컴파일되지 않게된다. 또한 두 점의 좌표가 다를 수 컴파일되지 않는 (전문화가 존재하지 않기 때문에). 라이브러리는 포인트 형에게 주어진 개념을 충족하는지 확인 할 수있다. 이것은 곧 Concept Checking 절에서 다루어진다.

반환 값 형식 (Return Type)
우리는 호출 언제든지 필수라고하는 것은 아니라고 약속했다. 그래서 double로 변환 가능한 제곱 값을 가진 distance result 구조체를 정의한다. 그러나 에만 정의된다. 구면에서의 거리 함수는 제곱근의 계산을하지 않는다. 따라서 상대적으로 비싼 제곱근의 계산을 방지 할 필요가 없기 때문이다. 그들은 단순히 거리를 돌려 준다. std::sqrtpythagoras

distance result 구조는 전략에 의존한다. 그래서 전략의 멤버 형식을 창조한다. result 구조는 다음과 같다 :

template < typename  T  =  double > 
struct  cartesian_distance 
{ 
    T  sq ; 
    explicit  cartesian_distance ( T  const &  v )  :  sq  ( v )  {}

    inline  operator  T ()  const 
    { 
        return  std :: sqrt ( sq ); 
    } 
};
제곱근을 계산하지 않고 자신과 다른 결과를 비교하는 연산자가 있을지도 모른다.

각각의 전략은 그 내부에 결과 형을 가져야한다. 예를 들면 :

typedef  cartesian_distance < T >  return_type ;
또는 :

typedef  double  return_type ;
등, 직교 좌표계 (pythagoras)와 구면 좌표계 각각.

다시 distance 함수는 새로운 결과 형에 대응하기 위해 수정된다. 전략에 의해 오버로드, 이것은 그다지 복잡하지 않다.

template  <  typename  G1 ,  typename  G2 ,  typename  Strategy  > 
typename  Strategy :: return_type  distance (  G1  const &  G1  ,  G2  const &  G2  ,  S  const &  strategy )
전략이없는 것을 제외하고, 우리는 전략과 좌표 값 형식을 선택해야한다. 할거야에는 일행이 너무 길어서 우리는 다른 메타 함수를 추가한다 :

template  < typename  G1 ,  typename  G2  =  G1 > 
struct  distance_result 
{ 
    typedef  typename  point_type < G1 > :: type  P1 ; 
    typedef  typename  point_type < G2 > :: type  P2 ; 
    typedef  typename  strategy_distance 
        < 
            typename  cs_tag < P1 > :: type , 
            typename  cs_tag < P2 > :: type ,
            P1 ,  P2 
        > :: type  S ;

    typedef  typename  S :: return_type  type ; 
};
그리고 distance 함수 수정 :

template  < typename  G1 ,  typename  G2 > 
inline  typename  distance_result < G1 ,  G2 > :: type  distance ( G1  const &  g1 ,  G2  const &  g2 ) 
{ 
    // ... 
}
물론 dispatch 전문화에서도이 같은 결과를 돌려 준다. 그들은 전략을 템플릿 매개 변수로 어디서나 가지고 가능한 한 중복되지 않는 버전이다.

요약 (Summary)
이 설계의 이론적 기초는 Boost.Geometry이 태그 디스패치, 개념, traits, 그리고 메타 프로그래밍에 의해 단계적으로 설계되어있다. 우리는 디자인을 보여주기 위해 잘 알려진 distance 함수를 사용했다.

Boost.Geometry 여기에서 언급 된 같은 것과 다른 몇 가지 기법 (automatically reversing template arguments, tag casting, and reusing implementation classes or dispatch classes as policies in other dispatch classes)으로 설계되어있다.  

<<<<<<<<<<<<<>>>>>>>>>>>>>    
  
## Boost.Geometry를 사용하여 2D 게임의 충돌 판정을 편리하게 하자
출처: https://joynote.hatenadiary.org/entry/20111009/1318171529  
    
### 선 끼리의 교점을 구한다  
  
```
namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> point;
bg::model::linestring<point> line1 = boost::assign::list_of<point>(0, 200)(300, 400);
bg::model::linestring<point> line2 = boost::assign::list_of<point>(100, 0)(100, 400);
std::deque<point> out;
bg::intersection( line1, line2, out); // 교점(100, 266.666687):point 타입이 out에 push_back 된다
bg::intersection( line1, line2, out); // 위와 같음。두번째 point가 push_back 된다
```
   

## touches 알고리즘
출처: https://faithandbrave.hateblo.jp/entry/20120718/1342597171  
  
이 함수는 자기 자신 혹은 2개의 지오메트리가 공간적으로 접촉하고 있는가를 판정하기 위해 사용한다.   

자기 접촉이 있는지 판정한다  
<img src="resource\20120718163035.png">   
  
```
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/assign/list_of.hpp>

namespace bg = boost::geometry;

typedef bg::model::d2::point_xy<int> point;
typedef bg::model::polygon<point> polygon;

int main()
{
    polygon x;
    bg::exterior_ring(x) = boost::assign::list_of<point>
                                (0, 0)(0, 3)(2, 3)
                                (2, 2)(1, 2)(1, 1)
                                (2, 1)(2, 2.5)(3, 2.5)
                                (3, 0)(0, 0);

    if (bg::touches(x)) {
        std::cout << "x is touch" << std::endl;
    }
    else {
        std::cout << "x is not touch" << std::endl;
    }
}
```
    
<pre>
x is touch
</pre>  
    
2개의 지오메트리가 접촉하고 있는지를 판정한다.  
<img src="resource\20120718163852.png">     
  
```
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/assign/list_of.hpp>

namespace bg = boost::geometry;

typedef bg::model::d2::point_xy<int> point;
typedef bg::model::polygon<point> polygon;

int main()
{
    polygon x;
    bg::exterior_ring(x) = boost::assign::list_of<point>
                                (  0,   0)
                                (  0, 100)
                                (100, 100)
                                (100,   0)
                                (  0,   0);

    polygon y;
    bg::exterior_ring(y) = boost::assign::list_of<point>
                                (100, 100)
                                (100, 200)
                                (200, 200)
                                (200, 100)
                                (100, 100);

    if (bg::touches(x, y)) {
        std::cout << "touch" << std::endl;
    }
    else {
        std::cout << "not touch" << std::endl;
    }
}
```  
  
<pre>
touch
</pre>  
    