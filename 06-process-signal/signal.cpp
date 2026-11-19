#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstdio>
void handler(int signo){
    const char message[] = "\nSIGINT 수신!\n";
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

    
    if(sigaction(SIGINT, &act , nullptr) == -1){
        std::perror("시그널 실패");
        return -1;
    }

    std::cout << "시그널 대기 중...\n";

    while(true){
        pause();
    }
    
    return 0;
}