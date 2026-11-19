#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>

int main() {

    int fd[2];

    if (pipe(fd) == -1) {
        std::perror("pipe 실패");
        return 1;
    }

    pid_t result = fork();

    if (result == -1) {
        std::perror("fork 실패");
        close(fd[0]);
        close(fd[1]);
        return 1;
    }

    if (result == 0) {
        std::cout << "자식프로세스 실행\n";

        close(fd[0]);  // 읽기용 닫기

        const char message[] = "Hello parent!";

        write(fd[1], message, sizeof(message) - 1);

        close(fd[1]);  // 쓰기용 닫기

    } else {
        std::cout << "부모프로세스 실행\n";

        close(fd[1]);  // 쓰기용 닫기

        char buffer[100]{};

        ssize_t n = read(fd[0], buffer, sizeof(buffer) - 1);

        if (n == -1) {
            std::perror("read 실패");
        } else if (n > 0) {
            buffer[n] = '\0';
            std::cout << buffer << "\n";
        }
        close(fd[0]);  // 읽기용 닫기

        waitpid(result, nullptr, 0);
    }

    return 0;
}