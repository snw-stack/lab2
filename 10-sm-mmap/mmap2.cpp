
#include <iostream>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main() {
    const char* srcPath = "input.txt";
    const char* dstPath = "output.txt";

    //  입력 파일 열기
    int srcfd = open(srcPath, O_RDONLY);

    if (srcfd == -1) {
        std::perror("open src");
        return 1;
    }

    // 입력 파일 크기 확인
    struct stat srcst{};

    if (fstat(srcfd, &srcst) == -1) {
        std::perror("fstat");
        close(srcfd);
        return 1;
    }

    off_t fileSize = srcst.st_size;

    // 일반 파일인지 확인
    if (!S_ISREG(srcst.st_mode)) {
        std::cerr << "입력 파일이 일반 파일이 아닙니다.\n";
        close(srcfd);
        return 1;
    }

    // 출력 파일 열기
    int dstfd = open(
        dstPath,
        O_CREAT | O_RDWR | O_TRUNC,
        0644
    );

    if (dstfd == -1) {
        std::perror("open dst");
        close(srcfd);
        return 1;
    }

    // 빈 파일 처리
    if (fileSize == 0) {
        std::cout << "빈 파일 복사 완료\n";
        close(srcfd);
        close(dstfd);
        return 0;
    }

    // 출력 파일 크기 조정
    if (ftruncate(dstfd, fileSize) == -1) {
        std::perror("ftruncate");
        close(srcfd);
        close(dstfd);
        return 1;
    }

    // 입력 파일 읽기 전용 매핑
    void* srcAddr = mmap(
        nullptr,
        static_cast<size_t>(fileSize),
        PROT_READ,
        MAP_PRIVATE,
        srcfd,
        0
    );

    if (srcAddr == MAP_FAILED) {
        std::perror("mmap src");
        close(srcfd);
        close(dstfd);
        return 1;
    }

    // 출력 파일 읽기/쓰기 매핑
    void* dstAddr = mmap(
        nullptr,
        static_cast<size_t>(fileSize),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        dstfd,
        0
    );

    if (dstAddr == MAP_FAILED) {
        std::perror("mmap dst");
        munmap(srcAddr, static_cast<size_t>(fileSize));
        close(srcfd);
        close(dstfd);
        return 1;
    }

    // 실제 데이터 복사
    std::memcpy(
        dstAddr,
        srcAddr,
        static_cast<size_t>(fileSize)
    );

    // 출력 매핑을 파일에 동기화
    int result = 0;

    if (msync(dstAddr, static_cast<size_t>(fileSize),
              MS_SYNC) == -1) {
        std::perror("msync");
        result = 1;
    }

    //  매핑 해제
    if (munmap(srcAddr, static_cast<size_t>(fileSize)) == -1) {
        std::perror("munmap src");
        result = 1;
    }

    if (munmap(dstAddr, static_cast<size_t>(fileSize)) == -1) {
        std::perror("munmap dst");
        result = 1;
    }

    // 파일 닫기
    if (close(srcfd) == -1) {
        std::perror("close src");
        result = 1;
    }

    if (close(dstfd) == -1) {
        std::perror("close dst");
        result = 1;
    }

    if (result == 0) {
        std::cout << "파일 복사 완료!\n";
    }

    return result;
}
