#include "Board.h"
#include "../common/ConsoleSetup.h"
#include "../common/ConsoleSetup.h"

const int outPadding = 2;
const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

Board::Board() {
    _hDimension = 10;
    _vDimension = 10;
    _letters.resize(_vDimension);
    for (size_t i = 0; i < _letters.size(); i++){
        _letters[i].resize(_hDimension);
        for(size_t j = 0; j < _letters[i].size(); j++){
            _letters[i][j] = ' ';
        }
    }

    _highlights.resize(_vDimension);
    for(auto &line : _highlights){
        line.resize(_hDimension);
        for(int i = 0; i < _hDimension; i++)
            line[i] = false;
    }
}

Board::Board(int nLines, int nCollumns) {
    _vDimension = nLines;
    _hDimension = nCollumns;
    _letters.resize(_vDimension);
    for (auto & line : _letters){
        line.resize(_hDimension);
        for(char & j : line){
            j = ' ';
        }
    }

    _highlights.resize(_vDimension);
    for(auto &line : _highlights){
        line.resize(_hDimension);
        for(int i = 0; i < _hDimension; i++)
            line[i] = false;
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
            codedWord entry;
            entry.firstCoord = line.substr(0, 2);
            entry.orientation = line.at(3);
            entry.word = line.substr(5);
            _words.push_back(entry);
            coord index = Board::getIndex(entry.firstCoord);

            if((entry.orientation == "H") or (entry.orientation == "h")){
                for(size_t w = 0; w < entry.word.size(); w++){
                    _letters[index.vLine][index.hCollumn + w] = entry.word.at(w);
                }
            }
            else if ((entry.orientation == "V") or (entry.orientation == "v")){
                for(size_t w = 0; w < entry.word.size(); w++){
                    _letters[index.vLine + w][index.hCollumn] = entry.word.at(w);
                }
            }
        }

        file.close();
    }
    else{
        int defaultSize = 20;
        _hDimension = defaultSize;
        _vDimension = defaultSize;
        _letters.resize(_vDimension);

        for (auto & _line : _letters){
            _line.resize(_hDimension);
            for(char & _letter : _line){
                _letter = ' ';
            }
        }
        std::cerr << "Cannot open file!" << std::endl;

    }

    _highlights.resize(_vDimension);
    for(auto &line : _highlights){
        line.resize(_hDimension);
        for(int i = 0; i < _hDimension; i++)
            line[i] = false;
    }
}

void Board::show() const { //Prototype function (needs styling)
    std::cout << "\n" << '\n' << std::string(outPadding, ' ');
    std::cout << " ";
    for (int i = 0; i < _hDimension; i++){
        std::cout << " " << alphabet.at(i);
    }
    std::cout << std::endl;
    for (int i = 0; i< _vDimension; i++){
        std::cout << std::string(outPadding, ' ');
        std::cout << std::string(1,(toupper(alphabet.at(i))));
        for(int j = 0; j < _hDimension; j++){
            std::cout << ' ';
            if (getHighlights().at(i).at(j)) {
                print(_letters.at(i).at(j), DEFAULT_FORE, RED_BACK);
            }
            else std::cout << _letters[i][j];
        }
        std::cout << '\n';
    }
}

coord Board::getIndex(std::string position) const {
    coord coordinates{};
    coordinates.vLine = alphabet.find(tolower(position.at(0)));
    coordinates.hCollumn = alphabet.find(position.at(1));
    return coordinates;
}

/*bool Board::fileExport(std::string filename) const {
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
}*/

bool Board::highlight(int vIndex, int hIndex) {
    if (vIndex >= (int) _vDimension || hIndex >= (int) _hDimension) return false;
    if (_letters.at(vIndex).at(hIndex) == ' ') return false;
    if (_highlights.at(vIndex).at(hIndex)) return false;
    _highlights.at(vIndex).at(hIndex) = 1;
    return true;
}

std::vector<std::vector<char>> Board::getLetters() const {
    return _letters;
}

std::vector<std::vector<bool>> Board::getHighlights() const {
    return _highlights;
}

coord Board::getDimensions() const {
    coord dimensions = { _hDimension, _vDimension };
    return dimensions;
}