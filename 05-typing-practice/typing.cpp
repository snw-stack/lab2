
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstdio>

int main(int argc, char* argv[]) {

    char ch;
    std::vector<std::string> sentences;
    std::vector<std::string> inputs;

    int errors = 0;
    std::string sentence;

    int fd = open("sentences.txt", O_RDONLY);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    // 파일에서 문장 읽기
    ssize_t n;

    while ((n = read(fd, &ch, 1)) == 1) {
        if (ch == '\n') {
            sentences.push_back(sentence);
            sentence.clear();
        } else {
            sentence.push_back(ch);
        }
    }

    if (n == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    if (!sentence.empty()) {
        sentences.push_back(sentence);
    }

    close(fd);

    // 입력받을 문자열 공간 확보
    inputs.resize(sentences.size());

    std::size_t all_input_size = 0;

    auto start = std::chrono::steady_clock::now();

    // 문장별 타자 연습
    for (std::size_t i = 0; i < sentences.size(); i++) {

        std::cout << sentences[i] << "\n";

        if (!std::getline(std::cin, inputs[i])) {
            std::cerr << "입력 오류\n";
            return 1;
        }

        all_input_size += inputs[i].size();

        std::size_t min_length =
            std::min(sentences[i].size(), inputs[i].size());

        // 같은 위치의 문자 비교
        for (std::size_t j = 0; j < min_length; j++) {

            if (sentences[i][j] != inputs[i][j]) {
                errors++;

                std::cout << sentences[i][j] << "와 "
                          << inputs[i][j]
                          << " 다른 문자 발견! "
                          << "누적 에러 수: " << errors << "\n";
            }
        }

        // 길이 차이 계산 (문장당 한 번)
        if (sentences[i].size() > inputs[i].size()) {
            errors += sentences[i].size() - inputs[i].size();
        } else if (sentences[i].size() < inputs[i].size()) {
            errors += inputs[i].size() - sentences[i].size();
        }
    }

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    double seconds = elapsed.count();

    double cpm = 0.0;

    if (seconds > 0.0) {
        cpm = all_input_size / seconds * 60.0;
    }

    std::cout << "\n측정 시간: " << seconds << "초\n";
    std::cout << "CPM: " << cpm << "\n";
    std::cout << "오타 횟수: " << errors << "\n";

    return 0;
}
