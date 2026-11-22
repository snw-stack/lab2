#include <mqueue.h>
#include <cstdio>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>

int main() {
    const char* queueName = "/lab2_chat";

    mqd_t qd = mq_open(queueName, O_RDONLY);

    if (qd == (mqd_t)-1) {
        std::perror("mq_open");
        return 1;
    }

    char buffer[129];

    ssize_t n = mq_receive(qd, buffer, 128, nullptr);

    if (n == -1) {
        std::perror("mq_receive");
        mq_close(qd);
        return 1;
    }

    buffer[n] = '\0';
    std::cout << "받은 메시지: " << buffer << '\n';

    mq_close(qd);

    if (mq_unlink(queueName) == -1) {
        std::perror("mq_unlink");
    }

    return 0;
}