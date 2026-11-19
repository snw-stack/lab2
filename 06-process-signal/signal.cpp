#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cstdio>
#include<sys/wait.h>
int received = 0;
void child_handler(int signo) {
    const char msg[] = "(자식) SIGUSR1 수신!\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    received = 1;
}

void parent_handler(int signo) {
    const char msg[] = "(부모) SIGUSR2 응답 수신!\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main(int argc , char* argv[]) {

    /**
    * int sigaction(
    *   int signo,
    *   const struct sigaction *act,
    *   struct sigaction *oact
    * );
    */

    struct sigaction pAct{};
    pAct.sa_handler = parent_handler;
    sigemptyset(&pAct.sa_mask);
    pAct.sa_flags = 0;

    struct sigaction cAct{};
    cAct.sa_handler = child_handler;
    sigemptyset(&cAct.sa_mask);
    cAct.sa_flags = 0;

    

    if(sigaction(SIGUSR2, &pAct , nullptr) == -1){
        std::perror("부모 시그널 실패");
        return -1;
    }
    if(sigaction(SIGUSR1, &cAct , nullptr) == -1){
        std::perror("자식 시그널 실패");
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
        waitpid(result , nullptr , 0); //자식 미아 방지 (자식이 종료되기 전까지 기다림)

        std::cout << "(부모) 부모 종료\n";
        
    }else{
        sigset_t blockMask;
        sigset_t oldMask;
        sigemptyset(&blockMask);
        sigaddset(&blockMask , SIGUSR1);
        //blockMask에 있는 시그널을 차단하고 , oldMask에 차단하지않은 시그널 저장
        sigprocmask(SIG_BLOCK, &blockMask, &oldMask);
        
        std::cout<< "자식 프로세스 실행" 
            << getpid() << '\n';

       

        while(!received){
            sigsuspend(&oldMask);
        }
        
        sigprocmask(SIG_UNBLOCK , &blockMask , nullptr);

        for(int i =0 ; i < 3; i++){
            sleep(1);
        }
     

        kill(getppid() ,SIGUSR2);
        std::cout << "(자식) 자식 종료\n";

    }

    return 0;
}