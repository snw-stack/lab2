#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>

int main() {

    int parentToChild[2];
    int childToParent[2];

    if (pipe(parentToChild) == -1) {
        std::perror("pipe 실패");
        return 1;
    }
    if (pipe(childToParent) == -1) {
        std::perror("pipe 실패");
        return 1;
    }


    pid_t pid = fork();

    if (pid == -1) {
        std::perror("fork 실패");
        close(parentToChild[0]); //부모 -> 자식 읽기
        close(parentToChild[1]); //부모 -> 자식 쓰기
        close(childToParent[0]); //자식 -> 부모 읽기
        close(childToParent[1]); //자식 -> 부모 쓰기
        return 1;
    }
    
    if(pid > 0 ){
        std::cout << "부모 프로세스 실행" << '\n';  

        close(childToParent[1]); //자식 -> 부모 쓰기
        close(parentToChild[0]); //부모 -> 자식 읽기

        const char message[] = "Hello child!";
        write(parentToChild[1] , message , sizeof(message)-1 ); //부모 -> 자식 쓰기

        char buffer[100]{};
        
        ssize_t n = read(childToParent[0], buffer, sizeof(buffer)-1); //자식 -> 부모 읽기

        if(n == -1){
            std::cerr << "read 실패" << '\n';
        }else if(n == 0){
            std::cerr << "데이터 없음, 모든 쓰기 끝 닫힘"<< '\n';
        }else if(n > 0){
            buffer[n] = '\0';
            std::cout << buffer << '\n';
        }

        close(parentToChild[1]);
        close(childToParent[0]);
        
        waitpid(pid,nullptr , 0);
    }else if(pid == 0){
        std::cout << "자식 프로세스 실행" << '\n'; 

        close(childToParent[0]); //자식 -> 부모 쓰기
        close(parentToChild[1]); //부모 -> 자식 읽기

        const char reply[] = "Hello parent!";
        write(childToParent[1] , reply , sizeof(reply)-1 ); //부모 -> 자식 쓰기

        char buffer[100]{};
        
        ssize_t n = read(parentToChild[0], buffer, sizeof(buffer)-1); //자식 -> 부모 읽기

        if(n == -1){
            std::cerr << "read 실패" << '\n';
        }else if(n == 0){
            std::cerr << "데이터 없음, 모든 쓰기 끝 닫힘"<< '\n';
        }else if(n > 0){
            buffer[n] = '\0';
            std::cout << buffer << '\n';
        }
    
        close(parentToChild[0]); 
        close(childToParent[1]); 
        
    }
    return 0;
}