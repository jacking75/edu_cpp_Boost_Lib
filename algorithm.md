## 수치 변화를 지정 범위 내로 하기
-  hp의 값이 최소 0 ~ maxHP 사이가 되도록 한다.
```
#include <boost/algorithm/clamp.hpp>

void hpchange(int addhp){
  hp = boost::algorithm::clamp(hp + addhp, 0, maxHp);
}
```
