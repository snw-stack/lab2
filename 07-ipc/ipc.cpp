#include <iostream>
#include <sys/file.h>
#include <fcntl.h>
#include <cstdio>
#include <unistd.h>

int main(int argc , char* argv[]){
    
    int fd = open("lab2.lock", O_WRONLY | O_CREAT | O_APPEND , 0644);

    if(fd ==-1){
        std::perror("open 실패");
        return -1;
    }
    
     std::cout << "PID " << getpid()
              << ": 잠금 요청\n" << std::flush;

    flock(fd , LOCK_EX);
    // flock()으로 LOCK_EX 잠금 획득
    // 오류 발생 시 처리하기

    std::cout << "PID " << getpid()
              << ": 잠금 획득!\n" << std::flush;

    sleep(5);

    flock(fd , LOCK_UN);

    std::cout << "PID " << getpid()
              << ": 잠금 해제\n";





    close(fd);
    return 0;

}