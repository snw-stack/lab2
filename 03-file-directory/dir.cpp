#include <iostream>
#include <string>
#include <dirent.h> //디렉토리 관련 함수들을 제공하는 헤더 파일

int main(int argc , char *argv[]){
    
    if(argc <2){
        std::cerr << "사용법: ./dir 디렉토리경로\n";
        return -1;
    }

    DIR * dir = opendir(argv[1]); //현재 디렉토리 열기 | 반환값 DIR객체는 해당 경로의 디렉토리들의 정보를 담고 있음
    
    if(dir == nullptr){
        std::cerr << "디렉토리 열기 실패\n";
        return -1;
    }
    
    struct dirent * entry;

    while((entry = readdir(dir)) != nullptr){ //opendir에서 반환된 DIR 객체를 이용해서 디렉토리의 내용을 읽어옴
        std::string name = entry -> d_name;

        if(name == "." || name == ".."){
            continue; //현재 디렉토리와 상위 디렉토리는 건너뜀
        }
        std::cout << "이름 :"<< entry -> d_name << "\n";
        std::cout << "inode: " << entry->d_ino << "\n";
        std::cout << "offset: " << entry->d_off << "\n";
        std::cout << "record length: " << entry->d_reclen << "\n";
        std::cout << "type: " << static_cast<int>(entry->d_type) << "\n";
        std::cout << "---------------------\n";
    }

    closedir(dir);

    return 0;
}