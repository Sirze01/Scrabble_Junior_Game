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
                              2);
}

bool dialogue(int &last, commandInterpreter aCommand){
    std::cout << std::string(1, '\n');
    if (last) {
        std::cout
                << stringWriter(100, "Please choose a valid command. If you need help input 'help'.", 2);
    }
    last++;
    std::cout << std::string(1, '\n');
    std::cout << std::string(2, ' ') << '(' << aCommand.boardName() << ") " << "Your input: ";
    std::string userInput;
    std::getline(std::cin, userInput);
    aCommand.edit(userInput);
    bool temp = aCommand.interpret(last);
    aCommand.cmdExit(last);
    return temp;
}

bool openingDialogue(int &last) {
    std::cout << std::string(1, '\n');
    if (last) {
        std::cout
                << stringWriter(100, "Please choose a valid command. If you need help input 'help'.", 2);
    }
    last++;
    std::cout << std::string(1, '\n');
    std::cout << std::string(2, ' ') << "Your input: ";
    std::string userInput;
    std::getline(std::cin, userInput);
    commandInterpreter command(userInput);
    bool temp = command.interpret(last);
    command.cmdExit(last);
    if (last != -1) {
        bool validation;
        do {
            do {
                validation = dialogue(last, command);
            } while (!validation);
        } while (last != -1);
    }

    return temp;
}


int main() {
    setupConsole();
    /*#################################################*/
    bool validation;
    openingMessage();
    int last = 0;
    do {
        do {
            validation = openingDialogue(last);
        } while (!validation);
    }while (last != -1);
    std::cout << "main exiting" << std::endl;

    return 0;
}
