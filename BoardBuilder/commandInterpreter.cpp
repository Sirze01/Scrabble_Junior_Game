#include "commandInterpreter.h"
#include <algorithm>
#include <sstream>


/**
 * Constructor - Creates a commandInterpreter object, being initializes with params through reference.
 * @param dict - Passed as reference, so the methods can modify it, it's a vector of strings with all the words in the dictionary
 * @param boardName - Passed as reference, is a string with the name of the board used to export it
 * @param board - Passed as reference, is a object of class board, containing the board to edit
 * @param dictOpen - Passed as reference,
 * @param boardOpen
 * @param command
 */
commandInterpreter::commandInterpreter(std::vector<std::string> &dict, std::string &boardName, Board &board,
        bool &dictOpen, bool &boardOpen, std::string command): _command(), _modifiers(), _dictOpen( dictOpen ),
        _boardOpen(boardOpen), _dict( dict ), _name(boardName), _board(board){

    stripSpaces(command);
    std::stringstream commandStream(command);
    std::string cmd;
    commandStream >> cmd;
    _modifiers = command.substr(cmd.size());
    if (!_modifiers.empty())
        _modifiers = _modifiers.substr(1);


    if ((cmd == "dict") or (cmd == "d"))
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


/**
 * Method to read user input to get a file path, saves it in the _modifiers variable
 */
void commandInterpreter::importPath() {
    std::cout << std::string(1, '\n');
    std::cout << stringWriter(100, "Input the path to the file you want to import", 2);
    std::cout << std::string(2, ' ') << "Path: ";
    std::getline(std::cin, _modifiers);
}

/**
 * Method to read user input to get a board Name, saves it in the _name variable
 */
void commandInterpreter::boardName() {
    std::cout << LEFT_PADDING_STR << "Insert the name of your board." << std::endl;
    std::cout << std::string(2, ' ') << "Board name: ";
    std::getline(std::cin, _name);
}


/**
 * Method to control the loop in the main function, to decide what messages to show
 *
 * @param statusCodes
 *      0 >  Valid command
 *      -1 Exit
 *      -2 loop, no message
 *      -3 invalid
 *      -4 delete
 */
void commandInterpreter::interpret(int &statusCodes) {

    if (_command == "new") {
        if (cmdNewBoard())
            statusCodes = 0;
        else
            statusCodes = -3;
    }

    else if(_command == "dict"){
        if(cmdDict())
            statusCodes = 0;
        else
            statusCodes = -3;
    }

    else if (_command == "import") {
        if (cmdImportBoard())
            statusCodes = 0;
        else
            statusCodes = -3;
    }

    else if (_command == "add") {
        if (cmdAdd())
            statusCodes = 0;
        else
            statusCodes = -3;
    }

    else if (_command == "remove") {
        if (cmdRemove())
            statusCodes = 0;
        else
            statusCodes = -3;
    }

    else if (_command == "export"){
            if(cmdExport())
                statusCodes = 0;
            else
                statusCodes = -3;

    }

    else if (_command == "help") {
        cmdHelp();
        statusCodes = -2;
    }

    else if (_command == "delete") {
        cmdDelete(statusCodes);
    }

    else if (_command == "exit") {
        cmdExit(statusCodes);
    }

    if (_command.empty())
        statusCodes = -3;
}


/**
 * Method to display tha available commands
 */
void commandInterpreter::cmdHelp(){
    std::vector<std::string> sentences = {
            "### Help Page - List of all available commands",
            "dict(d) 'filename' - Imports a board saved in a text file",
            "new(n) - Creates a new board",
            "import(i) 'filename' - Imports a board saved in a text file",
            "add(a) Xx H(V) 'Word' - Adds a word to the board, only can be used after opening a board",
            "remove {'Word'} - Removes a word to the board, only can be used after opening a board",
            "export(e) - Exports a board to a text file, only can be used after opening a board",
            "delete - Deletes the current board", "exit - Quit the program"

    };

    for (auto& sentence : sentences) {
        std::cout << LEFT_PADDING_STR << sentence << std::endl;
    }
}


/**
 * Method to import a dictionary (vector of valid words) to use in the board to edit
 * @return Returns true if the import is successful, false otherwise
 */
bool commandInterpreter::cmdDict() {
    if (_dictOpen && _boardOpen) {
        std::cout << std::string(BOARD_LEFT_PADDING, SPACE) << "You can't change your board's dictionary" << std::endl;
        return false;
    }

    if(_modifiers.empty()){
        importPath();
    }

    std::string line;
    std::ifstream file;
    file.open(_modifiers, std::ios::in);
    if (file.is_open()) {
        while(getline(file, line)){
            stripSpecialChars(line);
            if(line.size() > 1){
                _dict.push_back(line);
            }
        }
        file.close();
        _dictOpen = true;
        std::cout << std::string(BOARD_LEFT_PADDING, SPACE) << "Dictionary loaded\n" << std::endl;
        return true;
    }
    else{
        std::cout << '\n' << std::string(2, SPACE) << "Cannot open file, try another file\n" << std::endl;
        return false;
    }
}


/**
 * Method to create an edit the current Board to an empty one with the desired dimensions
 * @return Returns true if the board object is edited correctly, false otherwise
 */
bool commandInterpreter::cmdNewBoard() {
    if (_dictOpen) {
        if (_boardOpen) {
            std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Cannot create a new board. You already have one open" << std::endl;
            return false;
        }
    }
    else{
        std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Cannot create a new board. You haven't added a dictionary" << std::endl;
        return false;
    }

    std::string hTemp, vTemp;

    if (_modifiers.empty()) {
        std::string userInput;
        std::cout << std::endl;
        std::cout << LEFT_PADDING_STR << stringWriter(100, "What dimensions should the board be? (Height x Width)", 0);
        std::cout << std::string(2, ' ') << "Dimensions: ";
        std::getline(std::cin, _modifiers);
        std::cout << std::endl;
    }
    else {
        if (_modifiers.find_first_of(' ') !=_modifiers.find_last_of(' ')) {
            std::cout << LEFT_PADDING_STR << stringWriter(100, "Input the dimensions in a valid format", 0);
            return false;
        }
        hTemp = _modifiers;
        _modifiers = hTemp.substr(0, hTemp.find_first_of(' '));
        hTemp.erase(0, hTemp.find_first_of(' ') + 1);
        _name = hTemp.substr(0);
    }

    std::string tempStr;
    for(auto &character : _modifiers){
        if (character != ' ')
            tempStr += character;
    }
    _modifiers = tempStr;
    stripSpaces(_name);

    for (auto &atIndex : _modifiers) {
        if (!(((isalpha(atIndex) && toupper(atIndex) == 'X') || isdigit(atIndex)))) {
            std::cout << stringWriter(100, "Input the dimensions in a valid format", BOARD_LEFT_PADDING);
            return false;
        }
        if(isalpha(atIndex) && toupper(atIndex) == 'X')
            atIndex = toupper(atIndex);
    }

    vTemp = _modifiers.substr(0, _modifiers.find('X'));
    hTemp = _modifiers.substr(_modifiers.find('X') + 1);
    
    if ( std::stoul(vTemp) > _board.getAlphabet().size() || std::stoul(hTemp) > _board.getAlphabet().size()){
        _name = "";
        std::cout << std::string(1, '\n') << "The board you're trying to create is just too big. Create one up to 26x26" << std::endl;
        return false;
    }


    Board newBoard(std::stoul(vTemp), std::stoul(hTemp));
    if(_name.empty())
        boardName();
    _board = newBoard;
    _boardOpen = true;
    _board.show();
    return true;
}


/**
 * Method to import a previously created board from a external file correctly formated
 * @return Returns true if the board object is edited correctly, false otherwise
 */
bool commandInterpreter::cmdImportBoard() {
    if (!_dictOpen) {
        std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Cannot create a new board. You haven't added a dictionary" << std::endl;
        return false;
    }
    if (_boardOpen){
        std::cout << LEFT_PADDING_STR << "Cannot create a new board. You already have one open"
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
        importPath();
    }
    std::ifstream file;
    file.open(_modifiers, std::ios::in);
    if (!file.is_open()) {
        std::cout << LEFT_PADDING_STR << "Cannot open file! Try another\n" << std::endl;
        return false;
    }
    else{
        file.close();
    }
    Board newBoard(_modifiers);
    _board = newBoard;
    if(_name.empty()) {
        boardName();
    }
    _boardOpen = true;
    _board.show();
    return true;
}


/**
 * Method to add words to the board
 * @return Returns true if the word is palced, false otherwise
 */
bool commandInterpreter::cmdAdd() {
    if (!_boardOpen) {
        std::cout
                << stringWriter(100, "You need to be editing a board to run this command. Import or create a new one!",
                                2);
        return false;
    }


    codedWord newEntry = {{SIZE_MAX, SIZE_MAX}, '\0', ""};
    if(!_modifiers.empty()){
        std::string tempString;
        char tempChar;
        std::stringstream commandStream(_modifiers);
        commandStream >> tempString;
        if(isAlpha(tempString)){
            if (!(_board.boardBounds({_board.getIndex(tempString), '\0', ""}))) {
            return false;
            }
            newEntry.firstCoord = _board.getIndex(tempString);
        }
        else{
            return false;
        }
        tempString.erase();
        commandStream >> tempChar;
        newEntry.orientation = toupper(tempChar);
        if(!((newEntry.orientation == 'H') || (newEntry.orientation == 'V'))) {
            return false;
        }
        commandStream >> tempString;
        if(!tempString.empty()) {
            newEntry.word = tempString;
            if(!isAlpha(newEntry.word)) {
                return false;
            }
        }
    }

    else{
        std::string userInput;
        std::cout << LEFT_PADDING_STR << "Input the coordinates to the first letter of the word. " << std::endl;
        std::cout << LEFT_PADDING_STR << "Coordinate: ";
        getline(std::cin, userInput);
        stripSpaces(userInput);
        if(!(isAlpha(userInput) && _board.boardBounds({_board.getIndex(userInput), '\0', ""}))) {
            return false;
        }
        newEntry.firstCoord = _board.getIndex(userInput);

        std::cout << LEFT_PADDING_STR << "Input the desired orientation" << std::endl;
        std::cout << LEFT_PADDING_STR << "Orientation: ";
        getline(std::cin, userInput);
        stripSpaces(userInput);
        for(auto &letter: userInput) letter = toupper(letter);
        if (!(userInput.at(0) == 'H') || (userInput.at(0) == 'V')){
            return false;
        }
        newEntry.orientation = toupper(userInput.at(0));

        std::cout << LEFT_PADDING_STR << "Input the word you want to place" << std::endl;
        std::cout << LEFT_PADDING_STR << "Word: ";
        getline(std::cin, newEntry.word);
        if (!isAlpha(newEntry.word)){
            return false;
        }
    }


    for(auto &letter : newEntry.word) letter = toupper(letter);

    for (auto &entry : _board.getWords()) {
        if (entry.word == newEntry.word) {
            std::cout << stringWriter(100, "The word you're trying to add is already in the board",
                                      BOARD_LEFT_PADDING) << std::endl;
            return false;
        }
    }
    bool retValue = true;
    // Binary search
    for(auto &letter : newEntry.word) letter = tolower(letter);
    bool inDict = std::binary_search(_dict.begin(), _dict.end(), newEntry.word);
    for(auto &letter : newEntry.word) letter = toupper(letter);
    if (!inDict){
        std::cout <<stringWriter(100, "The word you chose isn't in the dictionary",
                                 BOARD_LEFT_PADDING).substr(0, std::string::npos - 1) << std::endl;
        retValue = false;
    }

    if (!(_board.boardBounds(newEntry))){
        std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "Word is out of range! Try another one." << std::endl;
        retValue = false;
    }
    else{
        retValue = _board.addWord(newEntry);
    }

    return retValue;
}


/**
 * Method to remove words to the board
 * @param statusCodes - Controls the external loop and the messages shown
 * @return Returns true if the word is removed, false otherwise
 */
bool commandInterpreter::cmdRemove() {
    if (!_boardOpen) {
        std::cout << LEFT_PADDING_STR <<
                stringWriter(100, "You need to be editing a board to run this command. Import or create a new one!",0);
        return false;
    }

    if(_modifiers.empty()) {
        std::cout << LEFT_PADDING_STR << "Word: ";
        getline(std::cin, _modifiers);
    }

    for (auto &letter : _modifiers) letter = toupper(letter);

    return _board.removeWord(_modifiers);
}


/**
 * Exports the current board to an external txt file
 * @return True if the export occurs correctly, false otherwise
 */
bool commandInterpreter::cmdExport() const{
    if(_board.fileExport((_name + ".txt"))) {
        std::cout << std::string(BOARD_LEFT_PADDING, SPACE) << "Board exported\n" << std::endl;
        return true;
    }
    return false;
}


/**
 * Controls the external loop to ask for new commands to edit the board, effectively creating a new one
 * @param statusCodes - Controls the external loop and the messages shown
 */
void commandInterpreter::cmdDelete(int &statusCodes) {
    _dictOpen = false;
    _boardOpen = false;
    _name = "";
    std::cout << std::string(BOARD_LEFT_PADDING, SPACE) << "Board deleted\n" << std::endl;
    statusCodes = -4;
}

/**
 * Exits the external loop
 * @param statusCodes - Controls the external loop and the messages shown
 */
void commandInterpreter::cmdExit(int &statusCodes) {
    statusCodes = -1;
}