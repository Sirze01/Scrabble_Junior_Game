#include <iostream>
#include "../common/ConsoleSetup.h"
#include "commandInterpreter.h"

/*#################################################################*/

void openingMessage() {
    const std::string message = "Board Builder v0.0.1";
    std::cout << std::string(2, ' ') << message << " Debug version\n";
    std::cout << std::string(2, ' ');
    for (int i = 0; i < 100; i++) {
        std::cout << '#';
    }
    std::cout << std::endl;
    std::cout << std::string(1, '\n');
    std::cout << std::string(2, ' ');
    for (int i = 0; i < (int) ((100 - message.size()) / 2); i++) { //size_t or cast to int to shut vs up
        std::cout << '-';
    }
    std::cout << message;
    for (int i = 0; i < (int) ((100 - message.size()) / 2); i++) {
        std::cout << '-';
    }
    std::cout << std::endl;
    std::cout << std::string(1, '\n');
    std::cout << std::string(2, ' ');
    for (int i = 0; i < 100; i++) {
        std::cout << '#';
    }
    std::cout << std::endl;
    std::cout << std::string(2, '\n');
    std::cout << stringWriter(100,
                              "Input the desired operation. Alternatively you can access the available commands with 'help' at anytime.",
                              BOARD_LEFT_PADDING);
}

void dictMessage() {
    std::cout << stringWriter(100,
                              "Start by importing a dictionary, so your board knows what words to use",
                              BOARD_LEFT_PADDING);
}

void boardMessage() {
    std::cout << stringWriter(100,
                              "Now you need to start editing a board. Try to create a new one with 'new' or import one already built with 'import'",
                              BOARD_LEFT_PADDING);
}

int main() {
    setupConsole();

    /*#################################################*/
    std::vector<std::string> dict;
    std::string boardName;
    Board board;
    bool dictOpen = false;
    bool boardOpen = false; // 0 to board closed, 1 to board open
    std::string userInput;
    int statusCodes = 0;
    openingMessage();
    int count = 0;
    do{
        do{
            if(count == 0) {
                dictMessage();
            }
            else if (count == 1) {
                boardMessage();
            }
            if (statusCodes == -3) {
                std::cout
                        << stringWriter(100, "Please choose a valid command. If you need help input 'help'.",
                                        BOARD_LEFT_PADDING);
            }
            if(boardName.empty())
                std::cout << std::string(2, BOARD_LEFT_PADDING) << "Your input: ";
            else
                std::cout << std::string(2, BOARD_LEFT_PADDING) << "(" << boardName << ") " << "Your input: ";
            std::getline(std::cin, userInput);
            commandInterpreter command(dict, boardName, board, dictOpen, boardOpen, userInput);
            command.interpret(statusCodes);
            if (dictOpen && count == 0 )
                count++;
            if (boardOpen && count == 1)
                count ++;
        }while(!(statusCodes == -1 || statusCodes == -4));
        count = 0;
    }while(statusCodes == -4);

    std::cout << "main exiting" << std::endl;
    return 0;
}
