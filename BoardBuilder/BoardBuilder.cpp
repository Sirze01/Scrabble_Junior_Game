#include <iostream>
#include "../common/Board.h"
#include "../common/Console_Setup.h"

int main() {
    #ifdef _WIN32
    SetupConsole();
    #endif
    const std::string message = "Board Builder v0.0.1";
    std::cout << "BordBuilder Debug version v0.0.1\n";
    for(int i = 0; i < 100; i++){
        std::cout << '#';
    }
    std::cout << std::endl;
    std::cout << std::string(1, '\n');
    for(int i = 0; i < ((100 - message.size()) / 2); i++){
        std::cout << '-';
    }
    std::cout << message;
    for(int i = 0; i < ((100 - message.size()) / 2); i++){
        std::cout << '-';
    }
    std::cout << std::endl;
    std::cout << std::string(1, '\n');
    for(int i = 0; i < 100; i++){
        std::cout << '#';
    }
    std::cout << std::endl;
    Board test("test.txt");

    test.show();
    test.highlight(0,1);
    test.show();

    return 0;
}
