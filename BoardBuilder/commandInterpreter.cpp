#include "commandInterpreter.h"


commandInterpreter::commandInterpreter() {
    _command = "";
    _modifiers = "";
}


commandInterpreter::commandInterpreter(std::string command) {
    std::string cmd = command.substr(0, command.find(' '));
    command.erase(0, cmd.size() + 1);
    _modifiers = command;
    if ((cmd == "new") or (cmd == "n"))
        _command = "new";

    else if ((cmd == "import") or (cmd == "i"))
        _command = "import";

    else if ((cmd == "add") or (cmd == "a"))
        _command = "add";

    else if ((cmd == "export") or (cmd == "e"))
        _command = "export";

    else if ((cmd == "help") or (cmd == "h"))
        _command = "help";

    else if (cmd == "exit")
        _command = "exit";

}


void commandInterpreter::edit(std::string command) {
    _command = "";
    _modifiers = "";
    std::string cmd = command.substr(0, command.find(' '));
    command.erase(0, command.find(' ') + 1);
    _modifiers = command;
    if ((cmd == "new") or (cmd == "n"))
        _command = "new";

    else if ((cmd == "import") or (cmd == "i"))
        _command = "import";

    else if ((cmd == "add") or (cmd == "a"))
        _command = "add";

    else if ((cmd == "export") or (cmd == "e"))
        _command = "export";

    else if ((cmd == "help") or (cmd == "h"))
        _command = "help";

    else if (cmd == "exit")
        _command = "exit";

}


std::string commandInterpreter::boardName() {
    return _name;
}


bool commandInterpreter::interpret(int &last) {
    if (_command == "new") {
        if (cmdNew() == true)
            last = 0;
        else{
            last = -3;
            return false;
        }

    } else if (_command == "import") {
        if (cmdImport() == true)
            last = 0;
        else
            return false;
    } else if (_command == "add") {
        if (cmdAdd() == true)
            last = 0;
        else
            return false;
    }
        /*else if (_command == "export"){
            if(cmdExport() == true)
                last = 0;
            else
                return false;
        }*/
    else if (_command == "help") {
        cmdHelp();
        last = -2;
    } else if (_command == "exit") {
        cmdExit(last);
    }

    return !_command.empty();
}


void commandInterpreter::cmdHelp() {
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


bool commandInterpreter::cmdNew() {
    if (_state) {
        std::cout << "Cannot create a new board. You already have one open" << std::endl;
        return false;
    }

    bool validation;
    int last = 0;
    std::string hTemp, vTemp;
    std::string userInput;



    if (_modifiers.empty()) {
        std::cout << std::string(1, '\n');
        std::cout << stringWriter(100, "What dimensions should the board be? (Width x Height)", 2);
        std::cout << std::string(1, '\n');
        std::cout << std::string(2, ' ') << "Dimensions: ";
        std::getline(std::cin, _modifiers);
    }
    else {
        if (_modifiers.find_first_of(' ') !=_modifiers.find_last_of(' '))
            return false;
        hTemp = _modifiers;
        _modifiers = hTemp.substr(0, hTemp.find_first_of(' '));
        hTemp.erase(0, hTemp.find_first_of(' '));
        if(!hTemp.empty())
            _name = hTemp.substr(hTemp.find_first_of(' ') + 1);
    }


    do {
        validation = true;
        if (last) {
            std::cout << stringWriter(100, "Input the dimensions in a valid format", 2);
            std::cout << std::string(2, ' ') << "Dimensions: ";
            std::getline(std::cin, _modifiers);
        }
        last++;
        hTemp = "";
        for(auto &letter :_modifiers){
            if (letter == ' '){
                continue;
            }
            hTemp += letter;
        }
        _modifiers = hTemp;


        for (auto &atIndex : _modifiers) {
            if (!(isdigit(atIndex) || atIndex == 'x' || atIndex == 'X')) {
                validation = false;
                break;
            }
        }

        if(_modifiers.find('x') != _modifiers.npos){
            hTemp = _modifiers.substr(0, _modifiers.find('x'));
            vTemp = _modifiers.substr(_modifiers.find('x') +1);
        }
        else{
            hTemp = _modifiers.substr(0, _modifiers.find('X'));
            vTemp = _modifiers.substr(_modifiers.find('X') + 1);
        }

    } while (!validation);

    if(_name.empty()) {
        std::cout << std::string(2, ' ') << "Insert the name of your board." << std::endl;
        std::cout << std::string(2, ' ') << "Board name: ";
        std::getline(std::cin, userInput);
        _name = userInput;
    }

    Board newBoard(std::stoi(hTemp), std::stoi(vTemp));
    _board = newBoard;
    _state = true;
    return true;
}


bool commandInterpreter::cmdImport() {
    if (_state)
        return false;
    std::cout << std::string(1, '\n');
    std::cout << stringWriter(100, "Input the file you want to import: ", 2);
    std::string userInput;
    std::cout << std::string(2, ' ') << "Path: ";
    std::getline(std::cin, userInput);
    std::ifstream file;
    file.open(userInput, std::ios::in);
    if (!file.is_open()) {
        bool exists = false;
        while (!exists) {
            std::cout << '\n' << std::string(2, ' ') << "Cannot open file, try another file" << std::endl;
            std::cout << std::string(2, ' ') << "Path: ";
            std::getline(std::cin, userInput);
            std::ifstream file;
            file.open(userInput, std::ios::in);
            exists = file.is_open();
        }
    }
    Board newBoard(userInput);
    _board = newBoard;
    std::cout << '\n' << std::string(2, ' ') << "Insert the name of your board." << std::endl;
    std::cout << std::string(2, ' ') << "Board name: ";
    std::getline(std::cin, userInput);
    _name = userInput;
    _state = true;
    return true;
}


bool commandInterpreter::cmdAdd() {
    if (!_state) {
        std::cout
                << stringWriter(100, "You need to be editing a board to run this command. Import or create a new one!",
                                2);
        return false;
    }
    _board.show();
    std::cout << "You've done it!" << std::endl;

    return true;
}


bool commandInterpreter::cmdExport() {
    return true;
}

void commandInterpreter::cmdExit(int &last) {
    if (_command == "exit")
        last = -1;
}