#include <iostream>
#include "../common/ConsoleSetup.h"
#include "commandInterpreter.h"

/*#################################################################*/

/**
 * Oppening message for the utility
 */
void openingMessage() {

    const std::string message = "Board Builder v0.0.1";
    const std::vector<char> separator(100, '#');
    const std::vector<char> filler(((100 - message.size()) / 2), '-');
    std::cout << LEFT_PADDING_STR << message << " Debug version\n";
    std::cout << LEFT_PADDING_STR;
    for(const auto &character : separator) std::cout << character;
    std::cout << std::string(1, '\n');
    std::cout << LEFT_PADDING_STR;
    for(const auto &character : filler) std::cout << character;
    std::cout << message;
    for(const auto &character : filler) std::cout << character;
    std::cout << std::endl;
    std::cout << std::string(1, '\n');
    std::cout << LEFT_PADDING_STR;
    for(const auto &character : separator) std::cout << character;
    std::cout << std::endl;
    std::cout << std::string(2, '\n');
    std::cout << LEFT_PADDING_STR << stringWriter(100,
                              "Input the desired operation. Alternatively you can access the available commands with 'help' at anytime.",0);
}


/**
 * Message about the need to open a dict when starting to edit a board
 */
void dictMessage() {
    std::cout << stringWriter(100,
                              "Start by importing a dictionary, so your board knows what words to use",
                              BOARD_LEFT_PADDING);
}


/**
 * Message about the need to create or import a board to edit
 */
void boardMessage() {
    std::cout << stringWriter(100,
                              "Now you need to start editing a board. Try to create a new one with 'new' or import one already built with 'import'",
                              BOARD_LEFT_PADDING);
}



/**
 * Main Function - Configures the Windows Console to accept ANSII escape codes and UTF-8 encoding, saves the board and
 * associated variables and loops the program according to the status codes
 * @return
 */
int main() {
    setupConsole();

    /*#################################################*/
    std::vector<std::string> dict;
    std::string boardName;
    Board board;
    bool dictOpen = false;
    bool boardOpen = false; // 0 to board closed, 1 to board open
    std::string userInput;

    /* 0 >  Valid command
    * -1 Exit
    * -2 loop, no message
    * -3 invalid
    * -4 delete
    * */
    int statusCodes = 0;

    openingMessage();
    int count = 0;
    do{
        do{
            if(count == 0) {
                dictMessage();
            }
            else if (count == 2) {
                boardMessage();
            }
            if (statusCodes == -3) {
                std::cout << LEFT_PADDING_STR << stringWriter(100, "Please choose a valid command. If you need help input 'help'.", 0);
            }
            if(boardName.empty()) {
                std::cout << LEFT_PADDING_STR << "Your input: ";
            }
            else {
                std::cout << LEFT_PADDING_STR << "(" << boardName << ") " << "Your input: ";
            }
            std::getline(std::cin, userInput);
            commandInterpreter command(dict, boardName, board, dictOpen, boardOpen, userInput);
            command.interpret(statusCodes);
            if (count == 0)
                count++;
            if (dictOpen && count == 1)
                count ++;
            if (count == 2)
                count++;
        }while(!(statusCodes == -1 || statusCodes == -4));
        count = 0;
    }while(statusCodes == -4);

    std::cout << "main exiting" << std::endl;
    return 0;
}
