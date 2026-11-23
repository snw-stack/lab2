#include <iostream>
#include <pthread.h>



void* hello_thread(void* arg){
    std::cout << "Hello, Thread!" << std::endl;
    
    return nullptr;
}

int main(){

    pthread_t tid;

    int result =pthread_create(
        &tid,
        nullptr,
        hello_thread,
        nullptr
    );

    if(result != 0){
        std::cerr << "오류번호:" << result << std::endl;
        return -1;
    }

    pthread_exit(nullptr);

    return 0;

}
