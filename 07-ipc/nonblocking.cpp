#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>

int main(){
    
    int parentToChild[2];
    //int childToParent[2];

    if (pipe(parentToChild) == -1) {
    std::perror("pipe 실패");
    return 1;
    }

    int cflags = fcntl(parentToChild[0], F_GETFL); //자식이 읽음
    fcntl(parentToChild[0] , F_SETFL , cflags | O_NONBLOCK);

    //int pflags = fcntl(childToParent[0], F_GETFL); //부모가 읽음
    //fcntl(childToParent[0] , F_SETFL , pflags | O_NONBLOCK);
  
    char buffer[100]{};

    ssize_t n = read(parentToChild[0] , buffer , sizeof(buffer)-1);

    if (n == -1 && errno == EAGAIN) {
        std::cout << "파이프에 데이터가 없습니다.\n";
    } else if (n == 0) {
        std::cout << "파이프 EOF\n";
    } else if (n > 0) {
        std::cout << "데이터를 읽었습니다.\n";
        buffer[n] = '\0';
        std::cout << buffer << '\n';
    } else {
        std::perror("read");
    }

    const char message[] = "Hello";

    write(parentToChild[1], message , sizeof(message) -1 );

    ssize_t a = read(parentToChild[0] , buffer , sizeof(buffer)-1);

    if (a == -1 && errno == EAGAIN) {
        std::cout << "파이프에 데이터가 없습니다.\n";
    } else if (a == 0) {
        std::cout << "파이프 EOF\n";
    } else if (a > 0) {
        std::cout << "데이터를 읽었습니다.\n";
        buffer[a] = '\0';
        std::cout << buffer << '\n';
    } else {
        std::perror("read");
    }

    close(parentToChild[1]);
    
    ssize_t c = read(parentToChild[0] , buffer , sizeof(buffer)-1);

    if (c == -1 && errno == EAGAIN) {
        std::cout << "파이프에 데이터가 없습니다.\n";
    } else if (c == 0) {
        std::cout << "파이프 EOF\n";
    } else if (c > 0) {
        std::cout << "데이터를 읽었습니다.\n";
        buffer[c] = '\0';
        std::cout << buffer << '\n';
    } else {
        std::perror("read");
    }

    close(parentToChild[0]);
    

    return 0;
}