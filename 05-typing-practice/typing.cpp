#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>

int main(int argc , char* argv[]){
    std::string sentence = "The quick brown fox";
    std::string input;

    int errors = 0;

    std::cout << "연습문장:" << sentence << "\n";

    auto start = std::chrono::steady_clock::now();

    std::cout << "입력: ";
    std::getline(std::cin, input);

    auto end = std::chrono::steady_clock::now();

    std::cout << input << "\n" ;
    
    //두 문자열 중 더 작은 문자 찾기
    std::size_t min_length = std::min(sentence.size() , input.size());
    std::cout << min_length << "\n";

    //반복문으로같은 위치의 문자 비교하기
    for(int i = 0 ; i < min_length ; i++){
        if(sentence[i] != input[i]){
            errors++;

            std::cout << sentence[i] << "와" << input[i]
            << "다른문자 발견! " 
            << "누적 에러 수: " << errors << "\n";
        }
    }

    if(sentence.size() > input.size() ){
        errors += sentence.size() - input.size();
    }else if(sentence.size() < input.size()){
        errors += input.size() - sentence.size();
    }else 
        std::cout << "문자수가 동일합니다." << "\n";
    

    std::chrono::duration<double> elapsed = end - start;
    double seconds = elapsed.count();
    
    double cpm = 0.0;

    if(seconds > 0.0){
        cpm = input.size() / seconds * 60 ;
    }


    std::cout << "측정 시간 :" << seconds << "\n";
    std::cout << "cpm : " << cpm << "\n";
    std::cout << "오타 횟수 :" << errors;
    
    return 0;
}