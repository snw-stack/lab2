#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd = open("test.txt", O_WRONLY | O_CREAT | O_APPEND, 0644); //open(파일이름 , 옵션 , 파일권한)

    if(fd == -1){
        std::cerr << "파일 열기 실패\n";
        return -1;
    }

    std::string message = "Hello, World!\n";

    ssize_t written = write(fd , message.c_str(),message.size());

    if(written == -1){
        std::cerr << "파일 쓰기 실패\n";
        close(fd);
        return -1;
    }

    if(static_cast<size_t>(written) != message.size()){
        std::cerr << " 일부 데이터만 기록됐습니다\n";
        close(fd);
        return -1;
    }

    std::cout << "파일에"<< written <<"바이트를 썼습니다.\n";
    
    close(fd);


    fd = open("test.txt", O_RDONLY);

    if(fd == -1){
        std::cerr << "파일 열기 실패\n";
        return -1;
    }    

    char buffer[100];

    ssize_t read_bytes = read(fd , buffer , sizeof(buffer)-1);

    if(read_bytes == -1){
        std::cerr << "파일 읽기 실패\n";
        close(fd);
        return -1;
    }

    std::cout.write(buffer, read_bytes);

    std::cout << "파일에서" << read_bytes <<"바이트를 읽었습니다.\n";
    close(fd);

    return 0;
}