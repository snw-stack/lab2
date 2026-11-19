#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstdio>
#include<sys/wait.h>
void handler(int signo){
    const char message[] = "SIGUSR1 수신!\n";
    write(STDOUT_FILENO , message , sizeof(message)-1);
}

int main(int argc , char* argv[]) {

    /**
    * int sigaction(
    *   int signo,
    *   const struct sigaction *act,
    *   struct sigaction *oact
    * );
    */

    struct sigaction act{};

    //시그널을 받았을 때 실행할 함수 지정
    act.sa_handler = handler;

    //핸들러 실행 중 추가로 차단할 시그널 집합 설정
    sigemptyset(&act.sa_mask);

    //시그널 처리 방식에 적용할 옵션
    act.sa_flags = 0;

    
    if(sigaction(SIGUSR1, &act , nullptr) == -1){
        std::perror("시그널 실패");
        return -1;
    }

    pid_t result = fork();


    if(result == -1){
        std::cerr << "fore실패\n";
        return -1;
    }


    if(result > 0){
        std::cout << "부모 프로세스 실행" 
            << getpid() << "\n";

        sleep(1);

        kill(result,SIGUSR1);
        waitpid(result,nullptr,0);
    }else{
        std::cout<< "자식 프로세스 실행" 
            << getpid() << '\n';
    
        for(int i =0; i < 5; i++){
            sleep(1);
        }

        std::cout << "(자식) 작업 종료\n";

    }

    return 0;
}