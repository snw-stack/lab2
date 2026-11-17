#include <iostream>
#include <string>
#include <dirent.h> //디렉토리 관련 함수들을 제공하는 헤더 파일
#include <sys/stat.h> //파일 정보를 가져오는 함수들을 제공하는 헤더 파일
#include <vector> //벡터 컨테이너를 제공하는 헤더 파일
#include <algorithm> //알고리즘 관련 함수들을 제공하는 헤더 파일


void list_directory(const std::string& path) {

    DIR* dir = opendir(path.c_str());

    if (dir == nullptr) {
        std::cerr << path << " 열기 실패\n";
        return;
    }

    struct dirent* entry;
    std::vector<std::string> names;

    //  모든 항목 수집
    while ((entry = readdir(dir)) != nullptr) {

        std::string name = entry->d_name;

        // ls 기본 동작처럼 숨김 파일 제외
        if (name[0] == '.') {
            continue;
        }

        names.push_back(name);
    }

    closedir(dir);

    //  파일명 정렬
    std::sort(names.begin(), names.end());

    //  현재 디렉토리 출력
    std::cout << path << ":\n";

    for (const auto& name : names) {
        std::cout << name << "  ";
    }

    std::cout << "\n\n";

    //  하위 디렉토리 탐색
    for (const auto& name : names) {

        std::string full_path = path + "/" + name;

        struct stat info;

        if (lstat(full_path.c_str(), &info) == -1) {
            std::cerr << full_path << " 정보 조회 실패\n";
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            list_directory(full_path);
        }
    }
}

int main(int argc , char* argv[]){

    if(argc <2 ){
        std::cerr << "사용법: ./recursivels 디렉토리경로\n";
        return -1;
    }

    list_directory(argv[1]);
    
    return 0;

}