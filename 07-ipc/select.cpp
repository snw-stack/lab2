#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include <cstdio>
#include <sys/wait.h>
int main(){

    int fd[2];
    
    if(pipe(fd) == -1){
        std::perror("pipe");
        return -1;
    }

    pid_t pid = fork();

    if(pid == -1){
        std::perror("fork");
        close(fd[0]);
        close(fd[1]);
        return -1;
    }
   
    if(pid == 0){
        std::cout << "자식 프로세스 실행" << "\n";
        
        close(fd[0]);

        sleep(2);
            
        const char message[] = "Hello";

        write(fd[1],message , sizeof(message)-1);

        close(fd[1]);

    }else{

        std::cout << "부모 프로세스 실행" << "\n";

        close(fd[1]);


        fd_set readSet;

        FD_ZERO(&readSet);
        FD_SET(fd[0],&readSet);

        std::cout << "부모: 데이터 대기 중...\n"
                  << std::flush;

        char buffer[100]{};



        int ready = select(
            fd[0]+1 , 
            &readSet , 
            nullptr , 
            nullptr , 
            nullptr
        );

        if(ready == -1){
            std::perror("select");
        }else if(FD_ISSET(fd[0], &readSet)){
            std::cout << "읽기 가능! \n";

            ssize_t n = read(
                fd[0],
                buffer,
                sizeof(buffer) -1
            );

            if(n == -1){
                std::perror("read");
            }
            else if (n == 0) {
                std::cout << "파이프 EOF\n";
            }
            else {
                buffer[n] = '\0';
                std::cout << buffer << '\n';
            }
            
        }

        close(fd[0]);

        waitpid(pid,nullptr , 0);
    }
    return 0;

}