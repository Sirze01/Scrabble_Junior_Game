#include <iostream>
#include "Board.h"

int main()
{
    const std::string message = "Board Builder v0.0.1";
    std::cout << "BordBuilder Debug version v0.0.1\n";
    for(int i = 0; i < 100; i++){
        std::cout << '#';
    }
    std::cout << std::endl;
    std::cout << std::string(1, '\n');
    for(int i = 0; i < 100; i++){
        std::cout << '#';
    }
    std::cout << std::string(1, '\n');
    
    return 0;
}
