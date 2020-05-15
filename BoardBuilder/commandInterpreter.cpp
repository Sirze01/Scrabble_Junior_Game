#include "commandInterpreter.h"
#include "../common/ConsoleSetup.h"
#include <algorithm>
const int outPadding = 2;

commandInterpreter::commandInterpreter() {
    _command = "";
    _modifiers = "";
}


commandInterpreter::commandInterpreter(std::string command) {
    std::string cmd = command.substr(0, command.find(' ')); //stripspaces
    command.erase(0, cmd.size());
    _modifiers = command.substr(command.find_first_of(' ') + 1);
    if (cmd == "dict")
        _command = "dict";

    else if ((cmd == "new") or (cmd == "n"))
        _command = "new";

    else if ((cmd == "import") or (cmd == "i"))
        _command = "import";

    else if ((cmd == "add") or (cmd == "a"))
        _command = "add";

    else if ((cmd == "remove") or (cmd == "r"))
        _command = "remove";

    else if ((cmd == "export") or (cmd == "e"))
        _command = "export";

    else if ((cmd == "help") or (cmd == "h"))
        _command = "help";

    else if ((cmd =="delete") or (cmd =="d"))
        _command = "delete";

    else if (cmd == "exit")
        _command = "exit";

}


void commandInterpreter::edit(std::string command) {
    _command = "";
    _modifiers = "";
    std::string cmd = command.substr(0, command.find(' ')); //stripspaces
    command.erase(0, cmd.size());
    _modifiers = command.substr(command.find_first_of(' ') + 1);
    if (cmd == "dict")
        _command = "dict";

    else if ((cmd == "new") or (cmd == "n"))
        _command = "new";

    else if ((cmd == "import") or (cmd == "i"))
        _command = "import";

    else if ((cmd == "add") or (cmd == "a"))
        _command = "add";

    else if ((cmd == "remove") or (cmd == "r"))
        _command = "remove";

    else if ((cmd == "export") or (cmd == "e"))
        _command = "export";

    else if ((cmd == "help") or (cmd == "h"))
        _command = "help";

    else if ((cmd == "delete") or (cmd == "d"))
        _command = "delete";

    else if (cmd == "exit")
        _command = "exit";

}


std::string commandInterpreter::boardName() { //get; const
    return _name;
}

/* 0  Valid command
 * -1 Exit
 * -2 loop, no message
 * -3 invalid
 * -4 delete
 * */
bool commandInterpreter::interpret(int &last) {

    if (_command == "dict"){
        if (cmdDict())
            last = 0;
        else{
            last = -3;
            return false;
        }

    }
    if (_command == "new") {
        if (cmdNew())
            last = 0;
        else{
            last = -3;
            return false;
        }
    }

    else if (_command == "import") {
        if (cmdImport())
            last = 0;
        else{
            last = -3;
            return false;
        }
    }

    else if (_command == "add") {
        if (cmdAdd(last))
            last = 0;
        else{
            return false;
        }
    }

    else if (_command == "remove") {
        if (cmdRemove(last))
            last = 0;
        else{
            return false;
        }
    }

    else if (_command == "export"){
            if(cmdExport())
                last = 0;
            else{
            last = -3;
            return false;
            }
    }

    else if (_command == "help") {
        cmdHelp();
        last = -2;
    }

    else if (_command == "delete") {
        cmdDelete(last);
    }

    else if (_command == "exit") {
        cmdExit(last);
    }

    if (_command.empty())
        last = -3;
    
    return !_command.empty();
}


void commandInterpreter::cmdHelp() const{
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

// testing
bool commandInterpreter::cmdDict() {
    if (_dictBool && _state) {
        std::cout << std::string(outPadding, ' ') << "Cannot change dicts. You already chose one for this board" << std::endl;
        return false;
    }

    if(_modifiers.empty()){
        std::cout << std::string(1, '\n');
        std::cout << stringWriter(100, "Input the file you want to import: ", 2);
        std::cout << std::string(2, ' ') << "Path: ";
        std::getline(std::cin, _modifiers);
    }
    std::string line;
    std::ifstream file;
    file.open(_modifiers, std::ios::in);
    if (file.is_open()) {
        while(getline(file, line)){
            if(line.size() > 1){
                stripSpecialChars(line);
                _dict.push_back(line);
            }
        }
        file.close();
    }
    else{
        bool exists = false;
        while (!exists) {
            std::cout << '\n' << std::string(2, ' ') << "Cannot open file, try another file" << std::endl;
            std::cout << std::string(2, ' ') << "Path: ";
            std::getline(std::cin, _modifiers);
            std::ifstream file;
            file.open(_modifiers, std::ios::in);
            exists = file.is_open();
        }
    }

    _dictBool = true;
    std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Dictionary loaded\n" << std::endl;
    return true;
}


bool commandInterpreter::cmdNew() {
    if (!_dictBool) {
        std::cout << std::string(outPadding, ' ') << "Cannot create a new board. You haven't added a dictionary" << std::endl;
        return false;
    }
    if (_state) {
        std::cout << std::string(outPadding, ' ') << "Cannot create a new board. You already have one open" << std::endl;
        return false;
    }

    bool validation;
    int last = 0;
    std::string hTemp, vTemp;
    std::string userInput;


    if (_modifiers.empty()) {
        std::cout << std::string(1, '\n');
        std::cout << stringWriter(100, "What dimensions should the board be? (Height x Width)", 2);
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

        if(_modifiers.find('x') != std::string::npos){
            vTemp = _modifiers.substr(0, _modifiers.find('x'));
            hTemp = _modifiers.substr(_modifiers.find('x') +1);
        }
        else{
            vTemp = _modifiers.substr(0, _modifiers.find('X'));
            hTemp = _modifiers.substr(_modifiers.find('X') + 1);
        }

    } while (!validation);

    if(_name.empty()) {
        std::cout << std::string(2, ' ') << "Insert the name of your board." << std::endl;
        std::cout << std::string(2, ' ') << "Board name: ";
        std::getline(std::cin, userInput);
        _name = userInput;
    }

    if (std::stoi(vTemp) > _board.getAlphabet().size() || std::stoi(hTemp) > _board.getAlphabet().size()){
        std::cout << std::string(1, '\n') << "The board you're trying to create is just too big. Create one up to 26x26" << std::endl;
        return false;
    }


    Board newBoard(std::stoi(vTemp), std::stoi(hTemp));
    _board = newBoard;
    _state = true;
    _board.show();
    return true;
}


bool commandInterpreter::cmdImport() {
    if (!_dictBool) {
        std::cout << std::string(outPadding, ' ') << "Cannot create a new board. You haven't added a dictionary" << std::endl;
        return false;
    }
    if (_state){
        std::cout << std::string(outPadding, ' ') << "Cannot create a new board. You already have one open"
                  << std::endl;
        return false;
    }
    if(!_modifiers.empty()){
        std::string temp;
        temp = _modifiers;
        _modifiers = _modifiers.substr(0, temp.find_first_of(' '));
        temp.erase(0, temp.find_first_of(' '));
        if (!temp.empty())
            _name = temp.substr(1);

    }
    else{
    std::cout << std::string(1, '\n');
    std::cout << stringWriter(100, "Input the file you want to import: ", 2);
    std::cout << std::string(2, ' ') << "Path: ";
    std::getline(std::cin, _modifiers);
    }
    std::ifstream file;
    file.open(_modifiers, std::ios::in);
    if (!file.is_open()) {
        bool exists = false;
        while (!exists) {
            std::cout << '\n' << std::string(2, ' ') << "Cannot open file, try another file" << std::endl;
            std::cout << std::string(2, ' ') << "Path: ";
            std::getline(std::cin, _modifiers);
            std::ifstream file;
            file.open(_modifiers, std::ios::in);
            exists = file.is_open();
        }
    }
    Board newBoard(_modifiers);
    _board = newBoard;
    if(_name.empty()) {
        std::cout << '\n' << std::string(2, ' ') << "Insert the name of your board." << std::endl;
        std::cout << std::string(2, ' ') << "Board name: ";
        std::getline(std::cin, _name);
    }
    _state = true;
    _board.show();
    return true;
}


bool commandInterpreter::cmdAdd(int &last) {
    if (!_state) {
        std::cout
                << stringWriter(100, "You need to be editing a board to run this command. Import or create a new one!",
                                2);
        return false;
    }

    bool retValue = true;
    codedWord newEntry = {{SIZE_MAX, SIZE_MAX}, '\0', ""};
    if(!_modifiers.empty()){
        if(isAlpha(_modifiers.substr(0,2))){
            if (!(_board.boardBounds({_board.getIndex(_modifiers.substr(0, 2)), '\0', ""}))) {
            last = -3;
            return false;
            }
            newEntry.firstCoord = {_board.getIndex(_modifiers.substr(0,2))};
            _modifiers.erase(0,2);
        }
        if(!_modifiers.empty()) {
            newEntry.orientation = toupper(_modifiers.substr(1, 1).at(0));
            _modifiers.erase(0, 2);
            if(!((newEntry.orientation == 'H') || (newEntry.orientation == 'V'))){
                last = -3;
                return false;
            }
        }
        if(!_modifiers.empty()) {
            newEntry.word = _modifiers.substr(1);
            if(!isAlpha(newEntry.word)) {
                last = -3;
                return false;
            }
        }
    }


    std::string userInput;
    if(newEntry.firstCoord.vLine == SIZE_MAX && newEntry.firstCoord.hColumn == SIZE_MAX){
        int temp = 0;
        do {
            if(temp)
                std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Your input is invalid, try again" << std::endl;
            else
                std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Input the coordinates to the first letter of the word. Press e to quit"
                      << std::endl;
            temp++;
            std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Coordinate: ";
            getline(std::cin, userInput);
            //use strip spaces!!!
        } while (!(isAlpha(userInput) && _board.boardBounds({_board.getIndex(userInput), '\0', ""})));
    }

    if(newEntry.orientation == '\0'){
        int temp = 0;
        do {
            if(temp)
                std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Your input is invalid, try again" << std::endl;
            else
                std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Input the desired orientation" << std::endl;
            temp++;
            std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Orientation: ";
            getline(std::cin, userInput);
            // Strip Spaces
            for(auto &letter: userInput) letter = toupper(letter);
        } while (!(userInput.at(0) == 'H') || (userInput.at(0) == 'V'));
    }

    if(newEntry.word.empty()){
        int temp = 0;
        do {
            if(temp)
                std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Your input is invalid, try again" << std::endl;
            else
                std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Input the word you want to place" << std::endl;
            temp++;
            std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Word: ";
            getline(std::cin, newEntry.word);
        } while (!isAlpha(newEntry.word));
    }


    for(auto &letter : newEntry.word) letter = toupper(letter);

    for (auto &entry : _board.getWords()) {
        if (entry.word == newEntry.word) {
            last = -3;
            std::cout << stringWriter(100, "The word you're trying to add is already in the board",
                                      BOARD_LEFT_PADDING) << std::endl;
            return false;
        }
    }

    // Binary search
    for(auto &letter : newEntry.word) letter = tolower(letter);
    bool inDict = std::binary_search(_dict.begin(), _dict.end(), newEntry.word);
    for(auto &letter : newEntry.word) letter = toupper(letter);
    if (!inDict){
        last = -2;
        std::cout <<stringWriter(100, "The word you chose isn't in the dictionary",
                                 BOARD_LEFT_PADDING).substr(0, std::string::npos - 1) << std::endl;
        retValue = false;
    }

    if (!(_board.boardBounds(newEntry))){
        last = -2;
        std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Word is out of range! Try another one." << std::endl;
        retValue = false;
    }
    else if(!(_board.checkIntersection(newEntry))){
        last = -2;
        std::cout <<stringWriter(100, "The word you're trying to add intersects with another in the wrong letter",
                                 BOARD_LEFT_PADDING).substr(0, std::string::npos - 1) << std::endl;
        retValue = false;
    }
    else if (!(_board.wordSpaces(newEntry))){
        last = -2;
        std::cout <<stringWriter(100, "The word you're trying to add doesn't fit in that space",
                                 BOARD_LEFT_PADDING).substr(0, std::string::npos - 1) << std::endl;
        retValue = false;
    }


    if(retValue){
        if (newEntry.orientation == 'V'){
            for(int i = 0; i < newEntry.word.size(); i++){
                _board.placeChar({newEntry.firstCoord.vLine + i, newEntry.firstCoord.hColumn}, newEntry.word.at(i));
            }
        }
        else{
            for(int i = 0; i < newEntry.word.size(); i++){
                _board.placeChar({newEntry.firstCoord.vLine, newEntry.firstCoord.hColumn + i}, newEntry.word[i]);
            }
        }
        _board.addWord(newEntry);
        _board.show();
    }
    return retValue;
}


bool commandInterpreter::cmdRemove(int &last) {
    if (!_state) {
        std::cout
                << stringWriter(100, "You need to be editing a board to run this command. Import or create a new one!",
                                2);
        return false;
    }

    if(_modifiers.empty()) {
        std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Word: ";
        getline(std::cin, _modifiers);
    }

    for (auto &letter : _modifiers) letter = toupper(letter);

    if(_board.wordExists(_modifiers)){
        codedWord wordToRemove = *_board.findWord(_modifiers);
        std::vector<coord> intersections = _board.checkIntersections(wordToRemove);
        if (wordToRemove.orientation== 'V'){
            for(int i = 0; i < wordToRemove.word.size(); i++){
                coord current = {wordToRemove.firstCoord.vLine + i, wordToRemove.firstCoord.hColumn};
                if(intersections.empty()){
                    _board.placeChar({wordToRemove.firstCoord.vLine + i, wordToRemove.firstCoord.hColumn}, ' ');
                }

                else{
                    if(std::find(intersections.begin(), intersections.end(), current) == intersections.end())
                        _board.placeChar({wordToRemove.firstCoord.vLine + i, wordToRemove.firstCoord.hColumn}, ' ');
                    }
                }
            }

        else{
            for(int i = 0; i < wordToRemove.word.size(); i++){
                coord current = {wordToRemove.firstCoord.vLine, wordToRemove.firstCoord.hColumn + i};
                if(intersections.empty()){
                    _board.placeChar({wordToRemove.firstCoord.vLine, wordToRemove.firstCoord.hColumn + i}, ' ');
                }

                else{
                    if(std::find(intersections.begin(), intersections.end(), current) == intersections.end())
                        _board.placeChar({wordToRemove.firstCoord.vLine + i, wordToRemove.firstCoord.hColumn}, ' ');
                }
            }
        }

        _board.removeWord(wordToRemove);
    }

    else {
        last = -2;
        std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "The word you trying to remove doesn't exist, try again"
                  << std::endl;
        return false;
    }
    _board.show();
    return true;
}


bool commandInterpreter::cmdExport() const{
    _board.fileExport((_name + ".txt"));
    return true;
}


void commandInterpreter::cmdDelete(int &last) {
    std::cout << "  Board deleted" << std::endl;
    last = -4;
}


void commandInterpreter::cmdExit(int &last) {
    last = -1;
}