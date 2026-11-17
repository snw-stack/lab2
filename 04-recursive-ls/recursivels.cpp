#include <iostream>
#include <string>
#include <dirent.h> //디렉토리 관련 함수들을 제공하는 헤더 파일
#include <sys/stat.h> //파일 정보를 가져오는 함수들을 제공하는 헤더 파일


void list_directory(const std::string& path){

    DIR * dir = opendir(path.c_str());

    if(dir == nullptr){
        std::cerr << path << "경로 디렉토리 열기 실패\n";
        return;
    }

    struct dirent * entry;
    
    while((entry = readdir(dir)) != nullptr){
        std::string name = entry -> d_name;
        

        if(name == "." || name == ".."){
            continue;
        }

        std::cout << "이름 :"<< entry -> d_name << "\n";
        std::cout << "inode: " << entry->d_ino << "\n";
        std::cout << "offset: " << entry->d_off << "\n";
        std::cout << "record length: " << entry->d_reclen << "\n";
        std::cout << "type: " << static_cast<int>(entry->d_type) << "\n";
        std::cout << "---------------------\n";

        std::string full_path = path + "/" + name;

        struct stat info;

        if(lstat(full_path.c_str(), &info) == -1){
            std::cerr << "파일 정보 가져오기 실패\n";
            continue;
        }

        if(S_ISDIR(info.st_mode)){
            
            list_directory(full_path);
        }

    }

    closedir(dir);

}

int main(int argc , char* argv[]){

    if(argc <2 ){
        std::cerr << "사용법: ./recursivels 디렉토리경로\n";
        return -1;
    }

    list_directory(argv[1]);
    
    return 0;

}