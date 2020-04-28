#include <iostream>
#include "../common/Board.h"
#include "../common/Console_Setup.h"

/*#################################################################*/


void oppeningMessage(){
    const std::string message = "Board Builder v0.0.1";
    std::cout << message << " Debug version\n";
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
}



bool oppeningDialogue(int &last){}



int main() {
    #ifdef _WIN32
    SetupConsole();
    #endif

    /*##############################*/
    bool validation;
    int last = 0;

    do {
        oppeningMessage();
        validation = oppeningDialogue(last);
    }while (!validation);
    last = 0;



    return 0;
}
