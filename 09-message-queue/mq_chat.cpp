
#include <iostream>
#include <mqueue.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <cstdio>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "사용법: ./mq_chat A 또는 B\n";
        return 1;
    }

    std::string role = argv[1];

    if (role != "A" && role != "B") {
        std::cerr << "A 또는 B를 입력하세요.\n";
        return 1;
    }

    struct mq_attr attr{};
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;

    mqd_t qAB = mq_open(
        "/lab2_a_to_b",
        O_CREAT | O_RDWR,
        0600,
        &attr
    );

    mqd_t qBA = mq_open(
        "/lab2_b_to_a",
        O_CREAT | O_RDWR,
        0600,
        &attr
    );

    if (qAB == (mqd_t)-1 || qBA == (mqd_t)-1) {
        std::perror("mq_open");

        if (qAB != (mqd_t)-1) mq_close(qAB);
        if (qBA != (mqd_t)-1) mq_close(qBA);

        return 1;
    }

    // 기존 큐가 있을 수도 있으므로 실제 속성을 확인
    struct mq_attr attrAB{}, attrBA{};

    if (mq_getattr(qAB, &attrAB) == -1 ||
        mq_getattr(qBA, &attrBA) == -1) {
        std::perror("mq_getattr");
        mq_close(qAB);
        mq_close(qBA);
        return 1;
    }

    // 역할에 따라 송신/수신 큐 지정
    mqd_t sendQ = (role == "A") ? qAB : qBA;
    mqd_t recvQ = (role == "A") ? qBA : qAB;

    size_t sendMax = static_cast<size_t>(
        (role == "A") ? attrAB.mq_msgsize : attrBA.mq_msgsize
    );

    size_t recvMax = static_cast<size_t>(
        (role == "A") ? attrBA.mq_msgsize : attrAB.mq_msgsize
    );

    std::vector<char> buffer(recvMax);
    std::string message;

    std::cout << role << " 채팅 시작!\n";
    std::cout << "종료하려면 exit 입력\n";

    while (true) {

        // A는 먼저 보내고, B는 먼저 받음
        if (role == "A") {

            std::cout << "A 메시지 입력: ";
            if (!std::getline(std::cin, message)) {
                message = "exit";
            }

            if (message.size() > sendMax) {
                std::cerr << "메시지가 너무 깁니다.\n";
                continue;
            }

            if (mq_send(sendQ, message.data(),
                        message.size(), 0) == -1) {
                std::perror("mq_send");
                break;
            }

            if (message == "exit") {
                break;
            }

            ssize_t n = mq_receive(
                recvQ, buffer.data(), buffer.size(), nullptr
            );

            if (n == -1) {
                std::perror("mq_receive");
                break;
            }

            message.assign(buffer.data(), n);

            if (message == "exit") {
                std::cout << "B가 채팅을 종료했습니다.\n";
                break;
            }

            std::cout << "B: " << message << '\n';

        } else {

            // B는 먼저 수신
            ssize_t n = mq_receive(
                recvQ, buffer.data(), buffer.size(), nullptr
            );

            if (n == -1) {
                std::perror("mq_receive");
                break;
            }

            message.assign(buffer.data(), n);

            if (message == "exit") {
                std::cout << "A가 채팅을 종료했습니다.\n";
                break;
            }

            std::cout << "A: " << message << '\n';

            // B가 답장
            std::cout << "B 메시지 입력: ";
            if (!std::getline(std::cin, message)) {
                message = "exit";
            }

            if (message.size() > sendMax) {
                std::cerr << "메시지가 너무 깁니다.\n";
                // A가 답장을 기다리므로 재입력
                while (message.size() > sendMax) {
                    std::cout << "B 메시지 재입력: ";
                    if (!std::getline(std::cin, message)) {
                        message = "exit";
                    }
                }
            }

            if (mq_send(sendQ, message.data(),
                        message.size(), 0) == -1) {
                std::perror("mq_send");
                break;
            }

            if (message == "exit") {
                break;
            }
        }
    }

    mq_close(qAB);
    mq_close(qBA);

    std::cout << "채팅 종료\n";

    return 0;
}
