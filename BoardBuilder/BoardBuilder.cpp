#include <iostream>
#include "../common/Board.h"
#include "../common/ConsoleSetup.h"
#include "../common/StringProcess.h"
#include "commandInterpreter.h"
bool openingDialogue(int &last);
/*#################################################################*/

void helpMessage() {

}

/*bool dialogue(Board &board, std::string boardName, int &last) {
    std::cout << std::string(2, '\n');
    if (!last)
        std::cout << stringWriter(100,
                                  "Input the desired operation. Alternatively you can access the available commands with 'help' at anytime.",
                                  2);
    else
        std::cout
                << stringWriter(100, "Please choose a valid command. If you need help input 'help'.", 2);
    last++;
    std::cout << std::string(1, '\n');
    std::cout << std::string(2, ' ') << '(' << boardName << ')' <<" Your input: ";
    std::string userInput;
    std::getline(std::cin, userInput);
    if ((userInput == "help") or (userInput == "h")) {
        helpMessage();
        last--;
        return false;
    }
    else if ((userInput == "new") or (userInput == "n")) {
        // Error (board already open)
        return false;
    }
    else if ((userInput == "import") or (userInput == "i")) {
        // Error (board already open)
        return false;
    }
    else if ((userInput == "add") or (userInput == "a")) {
        // Export function
        return true;
    }
    else if ((userInput == "export") or (userInput == "e")) {
        // Export function
        return true;
    }
    else if ((userInput == "back") or (userInput == "b")){
        int last = 0;
        openingDialogue(last);
        return true;
    }
    else if ((userInput == "exit"))
        return true;

    return false;
}*/



bool openingDialogue(int &last) {
    std::cout << std::string(2, '\n');
    if (!last)
        std::cout << stringWriter(100,
                                  "Input the desired operation. Alternatively you can access the available commands with 'help' at anytime.",
                                  2);
    else
        std::cout
                << stringWriter(100, "Please choose a valid command. If you need help input 'help'.", 2);
    last++;
    std::cout << std::string(1, '\n');
    std::cout << std::string(2, ' ') << "Your input: ";
    std::string userInput;
    std::getline(std::cin, userInput);
    if((userInput == "help") or (userInput == "h"))
        last--;
    commandInterpreter command(userInput);
    return !command.validation();
}

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

}


int main() {
    setupConsole();
    /*#################################################*/
    bool validation;
    int last = 0;
    openingMessage();
    do {
        validation = openingDialogue(last);
    } while (!validation);
    last = 0;
    std::cout << "main exiting" << std::endl;

    return 0;
}
