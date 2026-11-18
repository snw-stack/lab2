
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

// UTF-8 문자열을 문자 단위로 분리
std::vector<std::string_view> utf8Characters(std::string_view text) {
    std::vector<std::string_view> chars;

    for (std::size_t i = 0; i < text.size();) {
        const std::size_t start = i++;

        // UTF-8의 연속 바이트는 같은 문자에 포함
        while (i < text.size() &&
               (static_cast<unsigned char>(text[i]) & 0xC0) == 0x80) {
            ++i;
        }

        chars.push_back(text.substr(start, i - start));
    }

    return chars;
}

// 최소 편집 거리 계산 (삽입, 삭제, 치환)
std::size_t editDistance(std::string_view target,
                         std::string_view input) {
    const auto a = utf8Characters(target);
    const auto b = utf8Characters(input);

    std::vector<std::size_t> prev(b.size() + 1);
    std::vector<std::size_t> curr(b.size() + 1);

    std::iota(prev.begin(), prev.end(), 0);

    for (std::size_t i = 1; i <= a.size(); ++i) {
        curr[0] = i;

        for (std::size_t j = 1; j <= b.size(); ++j) {
            const std::size_t cost =
                (a[i - 1] == b[j - 1]) ? 0 : 1;

            curr[j] = std::min({
                prev[j] + 1,        // 삭제
                curr[j - 1] + 1,    // 삽입
                prev[j - 1] + cost  // 치환 또는 일치
            });
        }

        std::swap(prev, curr);
    }

    return prev[b.size()];
}

int main() {
    std::ifstream file("sentences.txt");

    if (!file) {
        std::cerr << "sentences.txt 파일을 열 수 없습니다.\n";
        return 1;
    }

    std::string target;
    std::string input;

    std::size_t totalChars = 0;
    std::size_t totalErrors = 0;
    std::size_t completedLines = 0;

    std::chrono::duration<double> totalTime{0};

    // 파일에서 한 줄씩 가져와 바로 타자 연습
    while (std::getline(file, target)) {

        // Windows CRLF 줄바꿈 처리
        if (!target.empty() && target.back() == '\r') {
            target.pop_back();
        }

        if (target.empty()) continue;

        std::cout << "\n[" << completedLines + 1 << "] "
                  << target << "\n입력> " << std::flush;

        // 사용자 입력 시간 측정
        const auto start = std::chrono::steady_clock::now();

        if (!std::getline(std::cin, input)) {
            break;
        }

        const auto end = std::chrono::steady_clock::now();

        // 결과 누적
        totalTime += end - start;
        totalChars += utf8Characters(input).size();

        const auto errors = editDistance(target, input);
        totalErrors += errors;

        ++completedLines;

        std::cout << "이번 문장 오타: "
                  << errors << "개\n";
    }

    if (completedLines == 0) {
        std::cout << "완료된 문장이 없습니다.\n";
        return 0;
    }

    // 최종 통계
    const double seconds = totalTime.count();

    const double cpm = seconds > 0.0
        ? totalChars * 60.0 / seconds
        : 0.0;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "\n=== 결과 ===\n";
    std::cout << "완료 문장 수: " << completedLines << "\n";
    std::cout << "입력 문자 수: " << totalChars << "\n";
    std::cout << "입력 시간: " << seconds << "초\n";
    std::cout << "CPM: " << cpm << "\n";
    std::cout << "총 오타: " << totalErrors << "개\n";

    return 0;
}
