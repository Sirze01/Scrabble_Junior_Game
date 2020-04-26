
#include "Board.h"

const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

// Private methods Definitions



// Public methods Definitions

Board::Board(int nLines, int nCollumns) {
    _vDimension = nLines;
    _hDimension = nCollumns;
    _letters.resize(_vDimension);
    for (size_t i = 0; i < _letters.size(); i++){
        _letters[i].resize(_hDimension);
        for(size_t j = 0; j < _letters[i].size(); j++){
            _letters[i][j] = ' ';
        }
    }
}



Board::Board(std::string filename)  {
    std::string line;
    std::ifstream file;
    file.open(filename);
    if(file.is_open()){
        getline(file, line);
        _vDimension = std::stoi(line.substr(0, 1));
        _hDimension= std::stoi(line.substr(5));
        _letters.resize(_vDimension);
        for (size_t i = 0; i < _letters.size(); i++){
            _letters[i].resize(_hDimension);
            for(size_t j = 0; j < _letters[i].size(); j++){
                _letters[i][j] = ' ';
            }
        }
        while(getline(file, line)){
            coord index;
            index = Board::getIndex(line.substr(0, 1));
            std::string word = line.substr(5);
            switch(line.at(3)){
                case 'H':
                    for(int w = 0; w < word.size(); w++){
                        _letters[index.vLine][index.hCollumn + w] = word.at(w);
                    }
                    break;
                case 'V':
                    for(int w = 0; w < word.size(); w++){
                        _letters[index.vLine + w][index.hCollumn] = word.at(w);
                    }
                    break;
            };
        }
        file.close();
    }
    else{
        std::cerr << "Cannot open file!" << std::endl;
    }
}



void Board::show() const {                              //Prototype function (needs styling)
    std::cout << ' ' << std::endl;
    for(size_t i = 0; i < _hDimension; i++){
        std::cout << alphabet.at(i);
    }
    std::cout << std::endl;
    for(size_t i = 0; i< _vDimension; i++){
        std::cout << toupper(alphabet.at(i));
        for(size_t j = 0; j < _hDimension; j++){
            std::cout << _letters[i][j];
        }
        std::cout << std::endl;
    }
}



coord Board::getIndex(std::string position) const {
    coord coordinates;
    coordinates.vLine = alphabet.find(tolower(position.at(0)));
    coordinates.hCollumn = alphabet.find(position.at(1));
    return coordinates;
}



void Board::fileExport(std::string filename) const {
    std::string line;


}



/*Board::Board() {
    _letters.resize(_vDimension);
    for (size_t i = 0; i < _letters.size(); i++){
        _letters[i].resize(_hDimension);
        for(size_t j = 0; j < _letters[i].size(); j++){
            _letters[i][j] = ' ';
        }
    }
}*/



/*Board::Board(std::vector<std::vector<char>> letters) {
    _vDimension = letters.size();
    _hDimension = letters[0].size();
    _letters = letters;
}*/