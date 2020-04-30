#include <iostream>
#include "../common/Board.h"
#include "../common/ConsoleSetup.h"

int main() {
    setupConsole();
    const std::string message = "Board Builder v0.0.1";
    std::cout << "BordBuilder Debug version v0.0.1\n";
    for(int i = 0; i < 100; i++){
        std::cout << '#';
    }
    std::cout << std::endl;
    std::cout << std::string(1, '\n');
    for(int i = 0; i < (int) ((100 - message.size()) / 2); i++){ //size_t or cast to int to shut vs up
        std::cout << '-';
    }
    std::cout << message;
    for(int i = 0; i < (int) ((100 - message.size()) / 2); i++){
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
