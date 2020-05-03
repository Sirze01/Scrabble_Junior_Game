#include <iostream>
#include "../common/Board.h"
#include "../common/ConsoleSetup.h"
#include "../common/StringProcess.h"
bool openingDialogue(int &last);
/*#################################################################*/

void helpMessage() {
    std::cout << std::string(2, '\n');
    std::cout << stringWriter(100, "### Help Page - List of all available commands", 2);
    std::cout << std::string(1, '\n');
    std::cout << stringWriter(100, "new(n) - Creates a new board", 2);
    std::cout << stringWriter(100, "import(i) 'filename' - Imports a board saved in a text file", 2);
    std::cout << stringWriter(100,
                              "add(a) Xx H(V) 'Word' - Adds a word to the board, only can be used after opening a board",
                              2);
    //std::cout << stringWriter(100, "remove Xx {H(V)} {'Word'} - Removes a word to the board, only can be used after opening a board", 2);
    std::cout << stringWriter(100,
                              "export(e) {'filename'} - Exports a board to a text file, only can be used after opening a board",
                              2);
    std::cout << stringWriter(100, "exit - Quit the program", 2);
}

bool dialogue(Board &board, std::string boardName, int &last) {
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
}




void boardManipNew() {
    std::cout << std::string(2, '\n');
    std::cout << stringWriter(100, "What dimensions should the board be? (Width x Height)", 2);
    std::cout << std::string(1, '\n');
    bool validation;
    int last = 0;
    std::string hTemp, vTemp;
    std::string userInput;
    do {
        validation = true;
        if (last)
            std::cout << stringWriter(100, "Input the dimensions in a valid format", 2);
        last++;
        std::cout << std::string(2, ' ') << "Dimensions: ";
        std::getline(std::cin, userInput);
        for (auto &letter : userInput) {
            if (!(isalnum(letter) or letter == ' ')) {
                validation = false;
                break;
            }
        }
        for (int i = 0; i < userInput.find(' '); i++) {
            if (!isdigit(userInput[i])) {
                validation = false;
                break;
            }
        }
        hTemp = userInput.substr(0, userInput.find(' '));
        userInput.erase(0, userInput.find(' ') + 1);
        if (!(userInput[0] == 'x' or userInput[0] == 'X')) {
            validation = false;
        }
        userInput.erase(0, 2);
        for (int i = 0; i < userInput.size(); i++) {
            if (!isdigit(userInput[i])) {
                validation = false;
                break;
            }
        }
        vTemp = userInput;
    } while (!validation);
    last = 0;
    Board newBoard(std::stoi(hTemp), std::stoi(vTemp));
    std::cout << std::string(2, ' ') << "Insert the name of your board." << std::endl;
    std::cout << std::string(2, ' ') << "Board name: ";
    std::getline(std::cin, userInput);
    do{
        validation = dialogue(newBoard, userInput, last);
    }while(!validation);
}

void boardManipImport() {
    std::cout << std::string(2, '\n');
    std::cout << stringWriter(100, "Input the file you want to import: ", 2);
    std::string userInput;
    std::cout << std::string(2, ' ') << "Path: ";
    std::getline(std::cin, userInput);
    std::ifstream file;
    file.open(userInput, std::ios::in);
    if(!file.is_open()){
        bool exists = false;
        while (!exists){
            std::cout << '\n' << std::string(2, ' ') << "Cannot open file, try another file" << std::endl;
            std::cout << std::string(2, ' ') << "Path: ";
            std::getline(std::cin, userInput);
            std::ifstream file;
            file.open(userInput, std::ios::in);
            exists = file.is_open();
        }
    }
    Board newBoard(userInput);
    int last = 0;
    std::cout << '\n' << std::string(2, ' ') << "Insert the name of your board." << std::endl;
    std::cout << std::string(2, ' ') << "Board name: ";
    std::getline(std::cin, userInput);
    bool validation;
    do{
        validation = dialogue(newBoard, userInput, last);
    }while(!validation);

}

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
    if ((userInput == "help") or (userInput == "h")) {
        helpMessage();
        last--;
        return false;
    }
    else if ((userInput == "new") or (userInput == "n")) {
        boardManipNew();
        return true;
    }
    else if ((userInput == "import") or (userInput == "i")) {
        boardManipImport();
        return true;
    }
    else if ((userInput == "add") or (userInput == "a")) {
        std::cout << "Please open a board, creating a new one or importing from a file." << std::endl;
        return false;
    }
    else if ((userInput == "export") or (userInput == "e")) {
        std::cout << "Please open a board, creating a new one or importing from a file." << std::endl;
        return false;
    }
    else if ((userInput == "exit"))
        return true;

    return false;
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
