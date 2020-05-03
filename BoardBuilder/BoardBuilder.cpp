#include <iostream>
#include "../common/Board.h"
#include "../common/ConsoleSetup.h"

<<<<<<< HEAD
/**Funtion to write strings across several lines
 *
 * @param text_width - Number of chars per line
 * @param text       - Text to write
 * @param padding    - Spaces before the line
 * */
std::string stringWriter(int text_width, std::string text, int padding = 0) {
    size_t charNbr;
    std::string output_str;

    while (!text.empty()) {
        output_str += std::string(padding, ' ');
        if (text.size() <= text_width) {
            output_str += (text + '\n');
            text.erase(0, text.size());
            continue;
        }

        charNbr = text_width - 1;
        if (text.at(charNbr) == '.' || text.at(charNbr) == ',' || text.at(charNbr) == '!' || text.at(charNbr) == '?' ||
            text.at(charNbr) == ';' || text.at(charNbr) == ' ') {
            output_str += (text.substr(0, charNbr + 1) + '\n');
            text.erase(0, charNbr + 1);
            continue;
        }


        if (isalnum(charNbr)) {
            if (text.at(charNbr + 1) == ' ') {
                output_str += (text.substr(0, charNbr + 1) + '\n');
                text.erase(0, charNbr + 1);
                text.erase(0, 1);
                continue;
            } else if (text.at(charNbr + 1) == '.' || text.at(charNbr + 1) == ',' || text.at(charNbr + 1) == '!' ||
                       text.at(charNbr + 1) == '?' || text.at(charNbr + 1) == ';') {
                while (isalnum(text.at(charNbr)) && isalnum(text.at(charNbr - 1))) {
                    charNbr--;
                }
                output_str += (text.substr(0, charNbr) + '\n');
                text.erase(0, charNbr);
                continue;
            } else {
                while (isalnum(text.at(charNbr)) && isalnum(text.at(charNbr - 1))) {
                    charNbr--;
                }
                output_str += (text.substr(0, charNbr) + '\n');
                text.erase(0, charNbr);
                continue;
            }
        }
    }
    return output_str;
}
/*#################################################################*/

void helpMessage(){
    std::cout << std::string(2, '\n');
    std::cout << stringWriter(100, "### Help Page - List of all available commands", 2);
    std::cout << std::string(1, '\n');
    std::cout << stringWriter(100, "new(n) - Creates a new board", 2);
    std::cout << stringWriter(100, "import(i) 'filename' - Imports a board saved in a text file", 2);
    std::cout << stringWriter(100, "add(a) Xx H(V) 'Word' - Adds a word to the board, only can be used after opening a board", 2);
    //std::cout << stringWriter(100, "remove Xx {H(V)} {'Word'} - Removes a word to the board, only can be used after opening a board", 2);
    std::cout << stringWriter(100, "export(e) {'filename'} - Exports a board to a text file, only can be used after opening a board", 2);
    std::cout << stringWriter(100, "exit - Quit the program", 2);
}



void boardManipNew(){
    std::cout << std::string(2, '\n');
    std::cout << stringWriter(100, "What dimensions should the board be? (Width x Height)", 2);
    std::cout << std::string(1, '\n');
    bool validation;
    int last = 0;
    do {
        if (last)
            std::cout << stringWriter(100, "Input the dimensions in a valid format", 2);
        last++;
        std::cout << std::string(2, ' ') << "Your input: ";
        std::string userInput;
        std::getline(std::cin, userInput);
        validation = false;
    }while (!validation);
    last = 0;


}



void openingMessage(){
=======
int main() {
    setupConsole();
>>>>>>> master
    const std::string message = "Board Builder v0.0.1";
    std::cout << std::string(2, ' ') << message << " Debug version\n";
    std::cout << std::string(2, ' ');
    for(int i = 0; i < 100; i++){
        std::cout << '#';
    }
    std::cout << std::endl;
    std::cout << std::string(1, '\n');
<<<<<<< HEAD
    std::cout << std::string(2, ' ');
    for(int i = 0; i < ((100 - message.size()) / 2); i++){
=======
    for(int i = 0; i < (int) ((100 - message.size()) / 2); i++){ //size_t or cast to int to shut vs up
>>>>>>> master
        std::cout << '-';
    }
    std::cout << message;
    for(int i = 0; i < (int) ((100 - message.size()) / 2); i++){
        std::cout << '-';
    }
    std::cout << std::endl;
    std::cout << std::string(1, '\n');
    std::cout << std::string(2, ' ');
    for(int i = 0; i < 100; i++){
        std::cout << '#';
    }
    std::cout << std::endl;
}


bool openingDialogue(int &last){
    std::cout << std::string(2, '\n');
    if (!last)
        std::cout << stringWriter(100, "Input the desired operation. Alternatively you can access the available commands with 'help' at anytime.", 2);
    else
        std::cout << stringWriter(100, "Invalid input. Please choose a valid command. If you need help input 'help'.", 2);
    last++;
    std::cout << std::string(1, '\n');
    std::cout << std::string(2, ' ') << "Your input: ";
    std::string userInput;
    std::getline(std::cin, userInput);
    if((userInput == "help") or (userInput == "h")){
        helpMessage();
        last--;
        return false;
    }
    else if ((userInput == "new") or (userInput == "n")){
        boardManipNew();
        return true;
    }
    else if ((userInput == "import") or (userInput == "i")){
        //importMessage();
        //importDialogue();
        return true;
    }
    else if ((userInput == "exit"))
        return true;

    return false;
}







int main() {
    //SetupConsole();


    /*##############################*/
    bool validation;
    int last = 0;
    openingMessage();
    do {
        validation = openingDialogue(last);
    }while (!validation);
    last = 0;
    std::cout << "main exiting" << std::endl;

    return 0;
}
