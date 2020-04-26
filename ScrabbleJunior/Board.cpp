#include <iostream>
#include "Board.h"
const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

// Private methods Definitions

Board::Board(std::vector<std::vector<char> > letters) {
    _letters = letters;
}


// Public methods Definitions

Board::Board() {
    _letters.resize(_dimension);
    for (size_t i = 0; i < _letters.size(); i++){
        _letters[i].resize(_dimension);
        for(size_t j = 0; j < _letters[i].size(); j++){
            _letters[i][j] = ' ';
        }
    }
}


void Board::show() const {                              //Prototype function (needs styling)
    std::cout << ' ' << std::endl;
    for(size_t i = 0; i < _dimension; i++){
        std::cout << alphabet.at(i);
    }
    std::cout << std::endl;
    for(size_t i = 0; i< _dimension; i++){
        std::cout << toupper(alphabet.at(i));
        for(size_t j = 0; j < _dimension; j++){
            std::cout << _letters[i][j];
        }
        std::cout << std::endl;
    }
}


coord Board::getIndex(std::string position) const {
    coord coordinates;
    coordinates.horizontal = alphabet.find(tolower(position.at(1)));
    coordinates.vertical = alphabet.find(position.at(2));
    return coordinates;
}


void Board::fileExport(std::string filename) const {

}