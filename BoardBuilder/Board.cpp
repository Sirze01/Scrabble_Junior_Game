#include "Board.h"
#include "../ScrabbleJunior/Command.h"

const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

// Private methods Definitions



// Public methods Definitions

Board::Board(int nLines, int nCollumns) {
    _vDimension = nLines;
    _hDimension = nCollumns;
    _letters.resize(_vDimension);
    for (auto & _letter : _letters){
        _letter.resize(_hDimension);
        for(char & j : _letter){
            j = ' ';
        }
    }
}



Board::Board(std::string filename)  {
    std::string line;
    std::ifstream file;
    file.open(filename, std::ios::in);

    if(file.is_open()){
        getline(file, line);
        _vDimension = std::stoi(line.substr(0, 2));
        _hDimension= std::stoi(line.substr(5));
        _letters.resize(_vDimension);

        for (auto & _line : _letters){
            _line.resize(_hDimension);
            for(char & _letter : _line){
                _letter = ' ';
            }
        }

        while(getline(file, line)){
            _words.push_back(line); //maybe implement _words as a vector of strings? {coord, orientation, word}
            coord index{};
            index = Board::getIndex(line.substr(0, 2));
            std::string word = line.substr(5);

            switch(line.at(3)){
                case 'H':
                    for(size_t w = 0; w < word.size(); w++){
                        _letters[index.vLine][index.hCollumn + w] = word.at(w);
                    }
                    break;
                case 'V':
                    for(size_t w = 0; w < word.size(); w++){
                        _letters[index.vLine + w][index.hCollumn] = word.at(w);
                    }
                    break;
            }
        }
        file.close();
    }
    else{
        std::cerr << "Cannot open file!" << std::endl;
        int defaultSize = 20;
        _hDimension = defaultSize;
        _vDimension = defaultSize;
    }

    std::vector<std::vector<bool>> binaries;
    for (int i = 0; i < (int)_vDimension; ++i) {
        std::vector<bool> binary;
        for (int j = 0; j < (int)_hDimension; ++j) {
            binary.push_back(0);
        }
        binaries.push_back(binary);
    }
    _highlights = binaries;
}



void Board::show() const { //Prototype function (needs styling)
    std::cout << " ";
    for (size_t i = 0; i < _hDimension; i++){
        std::cout << " " << alphabet.at(i);
    }
    std::cout << std::endl;
    for (size_t i = 0; i< _vDimension; i++){
        std::string line;
        line += toupper(alphabet.at(i));
        for(size_t j = 0; j < _hDimension; j++){
            line += " ";
            line += _letters[i][j];
        }
        line += '\n';
        std::cout << line;
    }
}



coord Board::getIndex(std::string position) const {
    coord coordinates{};
    coordinates.vLine = alphabet.find(tolower(position.at(0)));
    coordinates.hCollumn = alphabet.find(position.at(1));
    return coordinates;
}



bool Board::fileExport(std::string filename) const {
    /*OLD IMPLEMENTATION
    std::string line;
    std::ifstream file(filename);
    if (!file) {
        file.close();
        std::ofstream file(filename);
        if (file.is_open()) {
            file << _hDimension << 'x' << _vDimension << '\n';
            for (auto line : _words) {
                file << line << '\n';
            }
        }
        return true;
    }
    else {
        file.close();
        std::cerr << "The file already exists!" << std::endl;
        return false;
    }
    */

    std::string line;
    std::ofstream file (filename);
    if (file.is_open()){
        file << _hDimension << 'x' << _vDimension << '\n';
        for (auto line : _words){
            file << line << '\n';
        }
        return true;
    }
    std::cerr << "Could not write to file." << std::endl;
    return false;
}



Board::Board() {
    _hDimension = 20;
    _vDimension = 20;
    _letters.resize(_vDimension);
    for (size_t i = 0; i < _letters.size(); i++){
        _letters[i].resize(_hDimension);
        for(size_t j = 0; j < _letters[i].size(); j++){
            _letters[i][j] = ' ';
        }
    }
}

bool Board::move(Command command) {
    if (!(command.isMove())) return false;

    int vIndex = getIndex(command.getMove().at(0)).vLine;
    int hIndex = getIndex(command.getMove().at(0)).hCollumn;
    char letter = command.getMove().at(1).at(0);

    if (_letters.at(vIndex).at(hIndex) != letter) return false;
    if (_highlights.at(vIndex).at(hIndex) == 1) return false;

    //start or continue word
    if (hIndex > 0) {
        if (_letters.at(vIndex).at(hIndex - 1) != ' ') { //letter before at the line
            if (_highlights.at(vIndex).at(hIndex - 1) == 0) return false;
        }
    }
    if (vIndex > 0) {
        if (_letters.at(vIndex - 1).at(hIndex) != ' ') { //letter before at the collumn
            if (_highlights.at(vIndex - 1).at(hIndex) == 0) return false;
        }
    }

    _highlights.at(vIndex).at(hIndex) = 1;
    return true;
}