#include <iostream>
#include <unistd.h>
#include<sys/wait.h>
#include<cstdio>
int main(){

    std::cout << "fork 이전\n" << std::flush;

    pid_t result = fork();

    if(result == -1){
        std::cerr << "fork 실패" ;
        return -1;        
    }

    if(result > 0 ){
        std::cout << "(부모)부모 프로세스 실행: "  << getpid() << "\n";
        std::cout << "(부모)자식프로세스 pid: " << result << "\n";

         waitpid(result, nullptr , 0);

         std::cout << "자식 프로세스 종료 확인" << "\n";
    }else if(result == 0){
        
        std::cout << "(자식)자식 PID:" 
        << getpid() << "\n";
        
        std::cout << "exec호출 직전" << "\n";
        
        execl("/bin/ls","ls", "-l",
            static_cast<char*> (nullptr));

        std::perror("execl 실패");

        return 1;
    }

   

    
    return 0;

}