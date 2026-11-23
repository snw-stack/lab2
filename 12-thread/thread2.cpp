#include <iostream>
#include <pthread.h>

constexpr int NUM_THREADS = 3;

void* hello_thread(void* arg){
    int* number = static_cast<int*>(arg);
    
    std::printf("Tread  %d번째 : Hello World! \n", *number);

    return nullptr;
}


int main(){
    pthread_t tid[NUM_THREADS];

    static int ids[NUM_THREADS] = {0,1,2};
    
    for(int i = 0 ; i < NUM_THREADS ; i++){

        int result =pthread_create(
            &tid[i], // 생성된 스레드의 식별자를 저장할 주소 
            nullptr, // 스레드 속성 , nullptr이면 기본 속성
            hello_thread, // 새 스레드가 실행할 함수 
            &ids[i] //새 스레드 함수에 전달할 인자
        );

        if(result != 0){
            std::cerr << "오류번호:" << result << std::endl;
            return -1;
        }
    }   
    return 0;

}