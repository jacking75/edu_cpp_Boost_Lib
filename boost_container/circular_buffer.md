## circular_buffer  
![circular_buffer](resource/006.PNG "circular_buffer")    
  
- 순환 버퍼 라이브러리.
- FIFO 알고리즘 사용 가능.
- 고정 크기의 메모리 영역을 선 확보 후 그 영역을 링 형태로 사용.
- 할당한 크기 이내에서는 push_back, pop_back, pop_front는 O(1).
- 임의 접근 가능.
- 할당 한 크기를 넘으면 반대 측 끝을 재 사용.
  
![circular_buffer](resource/007.PNG "circular_buffer")     
  
### 에제
- circular_buffer_01
- circular_buffer_02
- circular_buffer_03
- circular_buffer_04
- circular_buffer_05  
  
    
