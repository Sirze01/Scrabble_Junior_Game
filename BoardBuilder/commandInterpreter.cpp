#include "commandInterpreter.h"
#include "../common/ConsoleSetup.h"
const int outPadding = 2;

commandInterpreter::commandInterpreter() {
    _command = "";
    _modifiers = "";
}


commandInterpreter::commandInterpreter(std::string command) {
    std::string cmd = command.substr(0, command.find(' '));
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
    std::string cmd = command.substr(0, command.find(' '));
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

    else if ((cmd == "export") or (cmd == "e"))
        _command = "export";

    else if ((cmd == "help") or (cmd == "h"))
        _command = "help";

    else if ((cmd == "delete") or (cmd == "d"))
        _command = "delete";

    else if (cmd == "exit")
        _command = "exit";

}


std::string commandInterpreter::boardName() {
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
        while(getline(file, line)){
            if(line.size() > 1){
                _dict.push_back(line);
            }
        }
        file.close();
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

/*
 * Needs to check if the word already exists in the board and ask for confirmation
 */
bool commandInterpreter::cmdAdd(int &last) {
    if (!_state) {
        std::cout
                << stringWriter(100, "You need to be editing a board to run this command. Import or create a new one!",
                                2);
        return false;
    }

    bool retValue = true;
    codedWord newEntry;
    if(!_modifiers.empty()){
        newEntry.firstCoord = _modifiers.substr(0,2);
        _modifiers.erase(0, 2);
        if(!((newEntry.firstCoord.size() == 2) && isAlpha(newEntry.firstCoord))){
            last = -3;
            return false;
        }
        if(!_modifiers.empty()) {
            newEntry.orientation = _modifiers.substr(1, 1);
            _modifiers.erase(0, 2);
            if(!((newEntry.orientation.size() == 1) && ((newEntry.orientation == "H") || (newEntry.orientation == "V") ||
                                                        (newEntry.orientation == "h") || (newEntry.orientation == "v")))){
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

    if(newEntry.firstCoord.empty()){
        int temp = 0;
        do {
            if(temp)
                std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Your input is invalid, try again" << std::endl;
            else
                std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Input the coordinates to the first letter of the word"
                      << std::endl;
            temp++;
            std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Coordinate: ";
            getline(std::cin, newEntry.firstCoord);
        } while (!((newEntry.firstCoord.size() == 2) && isAlpha(newEntry.firstCoord)));
    }

    if(newEntry.orientation.empty()){
        int temp = 0;
        do {
            if(temp)
                std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Your input is invalid, try again" << std::endl;
            else
                std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Input the desired orientation" << std::endl;
            temp++;
            std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Orientation: ";
            getline(std::cin, newEntry.orientation);
        } while (!((newEntry.orientation.size() == 1) && ((newEntry.orientation == "H") ||
        (newEntry.orientation == "V") || (newEntry.orientation == "h") || (newEntry.orientation == "v"))));
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

    newEntry.firstCoord[0] = toupper(newEntry.firstCoord[0]);
    newEntry.firstCoord[1] = tolower(newEntry.firstCoord[1]);
    for(auto &letter : newEntry.orientation) letter = toupper(letter);
    for(auto &letter : newEntry.word) letter = toupper(letter);

    // Binary search
    bool inDict = false;
    int first = 0, final = _dict.size() - 1, middle;
    for(auto &letter : newEntry.word) letter = tolower(letter);
    while (!inDict && first <= final){
        middle = (first + final) / 2;
        if (newEntry.word == _dict[middle].substr(0, _dict[middle].size() - 1)){
            inDict = true;
        }
        else if(_dict[middle].compare(newEntry.word) > 0)
            final = middle - 1;
        else
            first = middle + 1;
    }
    for(auto &letter : newEntry.word) letter = toupper(letter);
    if (!inDict){
        last = -2;
        std::cout <<stringWriter(100, "The word you chose isn't in the dictionary",
                                 BOARD_LEFT_PADDING).substr(0, std::string::npos - 1) << std::endl;
        retValue = false;
    }

    if (!(_board.boardBounds(_board.getIndex(newEntry.firstCoord), newEntry.orientation, newEntry.word.size()))){
        last = -2;
        std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Word is out of range! Try another one." << std::endl;
        retValue = false;
    }
    else if(!(_board.goodIntersects(newEntry))) {
        last = -2;
        std::cout << stringWriter(100, "The word you're trying to add intersects with another in the wrong letter",
                                  BOARD_LEFT_PADDING).substr(0, std::string::npos - 1) << std::endl;
        retValue = false;
    }
    else if(!(_board.wordSpaces(newEntry))){
        last = -2;
        std::cout << stringWriter(100, "The word you're trying to add doesn't fit in that space",
                                  BOARD_LEFT_PADDING).substr(0, std::string::npos - 1) << std::endl;
        retValue = false;
    }


    if(retValue){
        coord temp = _board.getIndex(newEntry.firstCoord);
        int v, h;
        v = temp.vLine;
        h = temp.hCollumn;
        if (newEntry.orientation == "V"){
            for(int i = 0; i < newEntry.word.size(); i++){
                temp.vLine = v + i;
                _board.lettersManip(temp, newEntry.word[i]);
            }
        }
        else{
            for(int i = 0; i < newEntry.word.size(); i++){
                temp.hCollumn = h + i;
                _board.lettersManip(temp, newEntry.word[i]);
            }
        }
        _board.addWord(newEntry);
        _board.show();
    }
    return retValue;
}


bool commandInterpreter::cmdExport() {
    return true;
}


void commandInterpreter::cmdDelete(int &last) {
    std::cout << "  Board deleted" << std::endl;
    last = -4;
}


void commandInterpreter::cmdExit(int &last) {
    last = -1;
}