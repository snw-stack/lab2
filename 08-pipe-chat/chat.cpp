#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <sys/wait.h>
#include <string>
#include <cerrno>
#include <csignal>

// 한 줄 읽기
bool readLine(int fd, std::string& buffer) {
    buffer.clear();
    char ch;

    while (true) {
        ssize_t n = read(fd, &ch, 1);

        if (n == 1) {
            if (ch == '\n') {
                return true;
            }

            buffer += ch;
        }
        else if (n == 0) {
            return false; // EOF
        }
        else if (errno == EINTR) {
            continue;
        }
        else {
            std::perror("read");
            return false;
        }
    }
}

// 한 줄 쓰기
bool writeLine(int fd, const std::string& message) {
    std::string data = message + '\n';
    size_t sent = 0;

    while (sent < data.size()) {
        ssize_t n = write(
            fd,
            data.data() + sent,
            data.size() - sent
        );

        if (n > 0) {
            sent += n;
        }
        else if (n == -1 && errno == EINTR) {
            continue;
        }
        else {
            std::perror("write");
            return false;
        }
    }

    return true;
}

int main() {

    // 상대가 파이프를 닫았을 때 EPIPE로 처리
    std::signal(SIGPIPE, SIG_IGN);

    int fd[2][2];

    for (int i = 0; i < 2; i++) {
        if (pipe(fd[i]) == -1) {
            std::perror("pipe");

            for (int j = 0; j < i; j++) {
                close(fd[j][0]);
                close(fd[j][1]);
            }

            return 1;
        }
    }

    pid_t pid = fork();

    if (pid == -1) {
        std::perror("fork");

        for (int i = 0; i < 2; i++) {
            close(fd[i][0]);
            close(fd[i][1]);
        }

        return 1;
    }

    if (pid == 0) { // 자식 프로세스

        close(fd[0][1]); // 부모 쓰기
        close(fd[1][0]); // 자식 읽기

        while (true) {
            std::string buffer;

            // 부모 메시지를 '\n'까지 읽기
            if (!readLine(fd[0][0], buffer)) {
                break;
            }

            if (buffer == "exit") {
                break;
            }

            std::cout << "부모: " << buffer << '\n';

            std::string message;

            std::cout << "(자식) 입력하세요: " << std::flush;

            if (!std::getline(std::cin, message)) {
                break;
            }

            // 부모에게 메시지 전송
            if (!writeLine(fd[1][1], message)) {
                break;
            }

            if (message == "exit") {
                break;
            }
        }

        close(fd[0][0]);
        close(fd[1][1]);

        return 0;

    } else { // 부모 프로세스

        close(fd[1][1]); // 자식 쓰기
        close(fd[0][0]); // 부모 읽기

        while (true) {
            std::string message;

            std::cout << "(부모) 입력하세요: " << std::flush;

            if (!std::getline(std::cin, message)) {
                break;
            }

            // 자식에게 메시지 전송
            if (!writeLine(fd[0][1], message)) {
                break;
            }

            if (message == "exit") {
                break;
            }

            std::string buffer;

            // 자식 메시지를 '\n'까지 읽기
            if (!readLine(fd[1][0], buffer)) {
                break;
            }

            if (buffer == "exit") {
                break;
            }

            std::cout << "자식: " << buffer << '\n';
        }

        close(fd[1][0]);
        close(fd[0][1]);

        while (waitpid(pid, nullptr, 0) == -1) {
            if (errno == EINTR) {
                continue;
            }

            std::perror("waitpid");
            break;
        }

        std::cout << "채팅 종료\n";
    }

    return 0;
}