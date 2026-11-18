#include <iostream>
#include <unistd.h>

int main(){

    std::cout << "fork 이전\n" << std::flush;

    pid_t result = fork();

    if(result == -1){
        std::cerr << "fork 실패" ;
        return -1;        
    }

    if(result > 0 ){
        std::cout << "부모 프로세스 실행: "  << getpid() << "\n";

    }else if(result == 0){
        std::cout << "자식프로세스 pid: " << getpid() ;
    }

    return 0;

}