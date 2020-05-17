#include <iostream>
#include "commandInterpreter.h"

/*#################################################################*/

/**
 * Oppening message for the utility
 */
void openingMessage(){
    std::stringstream output;
    const std::string message = "Board Builder v1.0.0";
    const std::string separator(100, '#');
    const std::string filler(((100 - message.size()) / 2), '-');

    std::cout << '\n' << LEFT_PADDING_STR << separator << "\n\n" <<
    LEFT_PADDING_STR << filler << message << filler << "\n\n  " << separator << "\n\n" << LEFT_PADDING_STR
    << "Input the desired operation.\n  Alternatively you can access the available commands with 'help' at  anytime.\n";

}


/**
 * Message about the need to open a dict when starting to edit a board
 */
void dictMessage() {
    Util::stringWriter("Start by importing a dictionary, so your board knows what words to use\n\n");
}


/**
 * Message about the need to create or import a board to edit
 */
void boardMessage() {
    Util::stringWriter("Now you need to start editing a board. Try to create a new one with 'new' or import one already built with 'import'\n\n");
}



/**
 * Main Function - Configures the Windows Console to accept ANSII escape codes and UTF-8 encoding, saves the board and
 * associated variables and loops the program according to the status codes
 * @return
 */
int main() {
    Util::setupConsole();

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
    //Counter to define what help message to show
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
                Util::stringWriter("Please choose a valid command. If you need help input 'help'.\n\n");
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
    return 0;
}
