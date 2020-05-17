#include "commandInterpreter.h"

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
                                       bool &dictOpen, bool &boardOpen, std::string &command)
        : _command(), _modifiers(), _dictOpen(dictOpen),
          _boardOpen(boardOpen), _dict(dict), _name(boardName), _board(board) {

    Util::stripSpaces(command);

    std::stringstream commandStream(command);
    std::string cmd;
    commandStream >> cmd;
    _modifiers = command.substr(cmd.size());
    Util::stripSpaces(_modifiers);

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
    Util::stringWriter("\nInput the path to the file you want to import\n");
    std::cout << LEFT_PADDING_STR << "Path: ";
    std::getline(std::cin, _modifiers);
}

/**
 * Method to read user input to get a board Name, saves it in the _name variable
 */
void commandInterpreter::boardName() {
    std::cout << LEFT_PADDING_STR << "Insert the name of your board." << std::endl
              << LEFT_PADDING_STR << "Board name: ";
    std::getline(std::cin, _name);
}


/**
 * Method to control the loop in the main function, to decide what messages to show
 *
 * @param statusCode
 *      0 >  Valid command
 *      -1 Exit
 *      -2 loop, no message
 *      -3 invalid
 *      -4 delete
 */
void commandInterpreter::interpret(int &statusCode) {

    if (_command == "new") {
        if (cmdNewBoard())
            statusCode = 0;
        else
            statusCode = -2;
    } else if (_command == "dict") {
        if (cmdDict())
            statusCode = 0;
        else
            statusCode = -2;
    } else if (_command == "import") {
        if (cmdImportBoard())
            statusCode = 0;
        else
            statusCode = -2;
    } else if (_command == "add") {
        if (cmdAdd(statusCode))
            statusCode = 0;
    } else if (_command == "remove") {
        if (cmdRemove(statusCode))
            statusCode = 0;
    } else if (_command == "export") {
        if (cmdExport())
            statusCode = 0;
        else
            statusCode = -2;

    } else if (_command == "help") {
        cmdHelp();
        statusCode = -2;
    } else if (_command == "delete") {
        cmdDelete(statusCode);
    } else if (_command == "exit") {
        cmdExit(statusCode);
    } else if (_command.empty())
        statusCode = -3;
}


/**
 * Method to display tha available commands
 */
void commandInterpreter::cmdHelp() {
    std::vector<std::string> sentences = {
            " ",
            "### Help Page - List of all available commands",
            "dict(d) 'filename' - Imports a board saved in a text file",
            "new(n) - Creates a new board",
            "import(i) 'filename' - Imports a board saved in a text file",
            "add(a) Xx H(V) 'Word' - Adds a word to the board, only can be used after opening a board",
            "remove {'Word'} - Removes a word to the board, only can be used after opening a board",
            "export(e) - Exports a board to a text file, only can be used after opening a board",
            "delete - Deletes the current board", "exit - Quit the program",
            " ",
    };

    std::stringstream output;
    for (const auto &sentence : sentences) {
        output << LEFT_PADDING_STR << sentence << std::endl;
    }
    std::cout << output.str();
}


/**
 * Method to import a dictionary (vector of valid words) to use in the board to edit
 * @return Returns true if the import is successful, false otherwise
 */
bool commandInterpreter::cmdDict() {
    if (_dictOpen && _boardOpen) {
        std::cout << LEFT_PADDING_STR << "You can't change your board's dictionary" << std::endl;
        return false;
    }

    if (_modifiers.empty()) {
        importPath();
    }

    std::string line;
    std::ifstream file;
    file.open(_modifiers, std::ios::in);
    // Check if file is open
    if (file.is_open()) {
        // Read from it while there are lines to read
        while (getline(file, line)) {
            Util::stripSpecialChars(line);
            if (line.size() > 1) {
                _dict.push_back(line);
            }
        }
        // Close the file and change flags
        file.close();
        _dictOpen = true;
        std::cout << LEFT_PADDING_STR << "Dictionary loaded\n\n";
        return true;
    } else {
        std::cout << LEFT_PADDING_STR << "Cannot open file, try another file\n\n";
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
            std::cout << LEFT_PADDING_STR << "Cannot create a new board. You already have one open\n\n";
            return false;
        }
    } else {
        std::cout << LEFT_PADDING_STR << "Cannot create a new board. You haven't added a dictionary\n\n";
        return false;
    }

    std::string hTemp, vTemp;

    if (_modifiers.empty()) { //assistant
        std::string userInput;
        Util::stringWriter("\nWhat dimensions should the board be? (Height x Width)\n");
        std::cout << LEFT_PADDING_STR << "Dimensions: ";
        std::getline(std::cin, _modifiers);
        std::cout << std::endl;
    } else { // one liner
        if (_modifiers.find_first_of(SPACE) != _modifiers.find_last_of(SPACE)) {
            Util::stringWriter("Input the dimensions in a valid format!\n\n");
            return false;
        }
        hTemp = _modifiers;
        _modifiers = hTemp.substr(0, hTemp.find_first_of(SPACE));
        hTemp.erase(0, hTemp.find_first_of(SPACE) + 1);
        _name = hTemp.substr(0);
    }

    std::string tempStr;
    for (const char &character : _modifiers) {
        if (character != SPACE)
            tempStr += character;
    }
    _modifiers = tempStr;
    Util::stripSpaces(_name);

    for (char &atIndex : _modifiers) {
        if (!(((isalpha(atIndex) && std::toupper(atIndex) == 'X') || isdigit(atIndex)))) {
            Util::stringWriter("Input the dimensions in a valid format!\n\n");
            return false;
        }
        if (isalpha(atIndex) && std::toupper(atIndex) == 'X')
            atIndex = static_cast<char>(std::toupper(atIndex));
    }

    vTemp = _modifiers.substr(0, _modifiers.find('X'));
    hTemp = _modifiers.substr(_modifiers.find('X') + 1);

    if (std::stoul(vTemp) > MAX_BOARD_SIZE || std::stoul(hTemp) > MAX_BOARD_SIZE) {
        _name = std::string(); //prevent input prompt of using the faulty boards name
        std::cout << "\nThe board you're trying to create is just too big. Create one up to 20x20\n" << std::endl;
        return false;
    }


    Board newBoard(std::stoul(vTemp), std::stoul(hTemp));
    if (_name.empty())
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
        std::cout << LEFT_PADDING_STR << "Cannot create a new board. You haven't added a dictionary" << std::endl;
        return false;
    }
    if (_boardOpen) {
        std::cout << LEFT_PADDING_STR << "Cannot create a new board. You already have one open\n" << std::endl;
        return false;
    }
    if (!_modifiers.empty()) {
        std::string temp;
        temp = _modifiers;
        _modifiers = _modifiers.substr(0, temp.find_first_of(SPACE));
        temp.erase(0, temp.find_first_of(SPACE));
        if (!temp.empty())
            _name = temp.substr(1);

    } else {
        importPath();
    }
    std::ifstream file;
    file.open(_modifiers, std::ios::in);
    // Check if file exists
    if (!file.is_open()) {
        std::cout << LEFT_PADDING_STR << "Cannot open file! Try another\n" << std::endl;
        return false;
    } else {
        // Close file if it exists
        file.close();
    }
    Board newBoard(_modifiers);
    _board = newBoard;
    if (_name.empty()) {
        boardName();
    }
    _boardOpen = true;
    _board.show();
    return true;
}


/**
 * Method to add words to the board
 * @param statusCode - Controls the external loop and the messages shown
 * @return Returns true if the word is palced, false otherwise
 */
bool commandInterpreter::cmdAdd(int &statusCode) {
    if (!_boardOpen) {
        statusCode = -2;
        Util::stringWriter("You need to be editing a board to run this command. Import or create a new one!\n\n");
        return false;
    }


    codedWord newEntry = {{SIZE_MAX, SIZE_MAX}, '\0', std::string()};
    if (!_modifiers.empty()) { //one liner command
        if (!Util::isAlpha(_modifiers, true)) {
            statusCode = -3;
            return false;
        }

        std::string tempString; //handle coords, orientation and word
        char tempChar;
        std::stringstream commandStream(_modifiers);
        commandStream >> tempString;
        newEntry.firstCoord = Board::getIndex(tempString);
        //tempString.erase(); //orientation
        commandStream >> tempChar;
        newEntry.orientation = static_cast<char>(std::toupper(tempChar));
        if (newEntry.orientation != 'H' && newEntry.orientation != 'V') {
            statusCode = -3;
            return false;
        }
        commandStream >> tempString;
        if (!tempString.empty()) {
            newEntry.word = tempString;
        }
    } else { //assistant
        std::string userInput;
        std::cout << LEFT_PADDING_STR << "Input the coordinates to the first letter of the word. " << std::endl
                  << LEFT_PADDING_STR << "Coordinate: ";
        getline(std::cin, userInput);
        Util::stripSpaces(userInput);

        if (!Util::isAlpha(userInput)) {
            statusCode = -3;
            return false;
        }
        newEntry.firstCoord = Board::getIndex(userInput);

        std::cout << LEFT_PADDING_STR << "Input the desired orientation" << std::endl
                  << LEFT_PADDING_STR << "Orientation: ";
        getline(std::cin, userInput);
        Util::stripSpaces(userInput);
        Util::upperCase(userInput);

        if (userInput.at(0) != 'H' && userInput.at(0) != 'V') {
            statusCode = -3;
            return false;
        }
        newEntry.orientation = static_cast<char>(std::toupper(userInput.at(0)));

        std::cout << LEFT_PADDING_STR << "Input the word you want to place" << std::endl
                  << LEFT_PADDING_STR << "Word: ";
        getline(std::cin, newEntry.word);
        if (!Util::isAlpha(newEntry.word)) {
            statusCode = -3;
            return false;
        }
    }

    Util::upperCase(newEntry.word);

    const std::vector<codedWord> words = _board.getWords();
    for (const auto &entry : words) {
        if (entry.word == newEntry.word) {
            statusCode = -2;
            Util::stringWriter("The word you're trying to add is already in the board\n\n");
            return false;
        }
    }

    Util::upperCase(newEntry.word);

    if (!_board.boardBounds(newEntry)) {
        statusCode = -2;
        Util::stringWriter("Word is out of range! Try another one.\n\n");
        return false;
    } else {
        std::string lowerWord = newEntry.word;
        Util::lowerCase(lowerWord);

        if (!std::binary_search(_dict.begin(), _dict.end(), lowerWord)) {
            statusCode = -2;
            Util::stringWriter("The word you chose isn't in the dictionary\n");
            return false;
        }

        return _board.addWord(newEntry, statusCode);
    }
}


/**
* Method to remove words to the board
* @param statusCode - Controls the external loop and the messages shown
* @return Returns true if the word is removed, false otherwise
*/
bool commandInterpreter::cmdRemove(int &statusCode) {
    if (!_boardOpen) {
        statusCode = -2;
        Util::stringWriter("You need to be editing a board to run this command. Import or create a new one!\n");
        return false;
    }

    if (_modifiers.empty()) {
        std::cout << LEFT_PADDING_STR << "Word: ";
        getline(std::cin, _modifiers);
    }

    Util::upperCase(_modifiers);

    return _board.removeWord(_modifiers, statusCode);
}


/**
 * Exports the current board to an external txt file
 * @return True if the export occurs correctly, false otherwise
 */
bool commandInterpreter::cmdExport() const {
    if (_board.fileExport((_name + ".txt"))) {
        std::cout << LEFT_PADDING_STR << "Board exported\n" << std::endl;
        return true;
    }
    return false;
}


/**
 * Controls the external loop to ask for new commands to edit the board, effectively creating a new one
 * @param statusCode - Controls the external loop and the messages shown
 */
void commandInterpreter::cmdDelete(int &statusCode) {
    _dictOpen = false;
    _boardOpen = false;
    _name = std::string();
    std::cout << LEFT_PADDING_STR << "Board deleted\n" << std::endl;
    statusCode = -4;
}

/**
 * Exits the external loop
 * @param statusCode - Controls the external loop and the messages shown
 */
void commandInterpreter::cmdExit(int &statusCode) {
    statusCode = -1;
}