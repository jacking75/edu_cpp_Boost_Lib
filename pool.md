### Pool
- 적당한 메모리를 처음부터 확보하고, 그기에서 적절한 메모리를 할당 받는다.
- 고속으로 메모리를 할당할 수 있다.
- 그러나 사용할 수 있는 곳이 한정되어 있다.
- 대량의 작은 오브젝트를 관리해야 하는 경우에 아주 좋다.
- 게임의 오브젝트 관리나 메모리 제어가 아주 중요한 곳에서 주로 사용한다.


### 오브젝트와 싱글톤
- 적당한 메모리를 처음부터 확보하고, 그기에서 적절한 메모리를 할당 받는다.
- 고속으로 메모리를 할당할 수 있다.
- 그러나 사용할 수 있는 곳이 한정되어 있다.
- 대량의 작은 오브젝트를 관리해야 하는 경우에 아주 좋다.
- 게임의 오브젝트 관리나 메모리 제어가 아주 중요한 곳에서 주로 사용한다.

### 간단 사용 법
- 오브젝트 풀

  ```
  struct Abc
  {
  	Abc()  { static int no=0; cout << "Abc() :" << (m_no=++no) << endl; }
  	~Abc() { cout << "~Abc():" << m_no << endl; }
  	int m_no;
  };

  boost::object_pool<Abc> p;
  Abc* x = p.construct();
  Abc* y = p.construct();
  Abc* z = p.construct();
  p.destroy(y);// 명시적으로 파괴자 호출
  ```

- 싱글톤 풀
```
// sizeof(int) 바이트 전용 할당자
boost::pool<> p( sizeof(int) );
int* x = (int*)p.malloc();
int* y = (int*)p.malloc();
int* z = (int*)p.malloc();
*x = 10;
p.free(z); // 명시적으로 free
```
