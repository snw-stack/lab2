
#include <iostream>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <cstdio>

int main() {

    const char* filename = "input.txt";

    //  파일 열기
    int fd = open(filename, O_RDONLY);

    if (fd == -1) {
        std::perror("open");
        return 1;
    }

    //  파일 크기 확인
    struct stat st{};

    if (fstat(fd, &st) == -1) {
        std::perror("fstat");
        close(fd);
        return 1;
    }

    off_t fileSize = st.st_size;

    if (fileSize <= 0) {
        std::cerr << "파일이 비어 있거나 크기가 올바르지 않습니다.\n";
        close(fd);
        return 1;
    }

    // 파일을 가상 주소 공간에 매핑
    void* addr = mmap(
        nullptr,
        static_cast<size_t>(fileSize),
        PROT_READ,
        MAP_PRIVATE,
        fd,
        0
    );

    if (addr == MAP_FAILED) {
        std::perror("mmap");
        close(fd);
        return 1;
    }

    //  반환된 가상 주소로 데이터 접근
    const char* data = static_cast<const char*>(addr);

    size_t n = std::min(
        static_cast<size_t>(fileSize),
        static_cast<size_t>(32)
    );

    std::cout.write(data, n);
    std::cout << '\n';

    //  매핑 해제
    if (munmap(addr, static_cast<size_t>(fileSize)) == -1) {
        std::perror("munmap");
        close(fd);
        return 1;
    }

    //  파일 디스크립터 닫기
    close(fd);

    return 0;
}
