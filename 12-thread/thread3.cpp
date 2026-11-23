#include<cstdio>
#include<pthread.h>

void * worker(void * arg){
    int * number = static_cast<int*> (arg);

    std::printf("Worker: received %d \n", *number);
    std::printf("Worker: finished\n");

    return arg;
}

int main(){

    pthread_t tid;
    int value = 10;
    void* result = nullptr;

    int ret = pthread_create(
        &tid,
        nullptr,
        worker,
        &value
    );

    if(ret != 0){
        std::printf("Error: %d\n", ret);
        return -1;
    }

    std::printf("Main: waiting for worker\n");

    int join = pthread_join(tid, &result);

    if(join != 0){
        std::printf("Error: %d\n", join);
        return -1;
    }

    std::printf("Main: worker returned %d\n", *static_cast<int*>(result));  
    
    std::printf("Main: finished\n"); 
    
    return 0;
}