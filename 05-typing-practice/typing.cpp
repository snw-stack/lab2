#include <iostream>
#include <string>

int main(int argc , char* argv[]){
    std::string sentence = "The quick brown fox";
    std::string input;

    std::cout << "sentence : ";

    std::getline(std::cin, input);

    std::cout << input;
    
    
    return 0;
}