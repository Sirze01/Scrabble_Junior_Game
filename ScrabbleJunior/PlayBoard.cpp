#include "PlayBoard.h"
#include "Command.h"

PlayBoard::PlayBoard(std::string filename) {
    std::string line;
    std::ifstream file;
    file.open(filename, std::ios::in);

    if (file.is_open()) {
        getline(file, line);
        _vDimension = std::stoi(line.substr(0, 2));
        _hDimension = std::stoi(line.substr(5));
        _letters.resize(_vDimension);

        for (auto& _line : _letters) {
            _line.resize(_hDimension);
            for (char& _letter : _line) {
                _letter = ' ';
            }
        }

        while (getline(file, line)) {
            _words.push_back(line); //maybe implement _words as a vector of strings? {coord, orientation, word}
            coord index{};
            index = Board::getIndex(line.substr(0, 2));
            std::string word = line.substr(5);

            switch (line.at(3)) {
            case 'H':
                for (size_t w = 0; w < word.size(); w++) {
                    _letters[index.vLine][index.hCollumn + w] = word.at(w);
                }
                break;
            case 'V':
                for (size_t w = 0; w < word.size(); w++) {
                    _letters[index.vLine + w][index.hCollumn] = word.at(w);
                }
                break;
            }
        }
        file.close();
    }
    else {
        std::cerr << "Cannot open file!" << std::endl;
        int defaultSize = 20;
        _hDimension = defaultSize;
        _vDimension = defaultSize;
    }

    //specific for PlayBoard
    std::vector<std::vector<bool>> binaries;
    for (int i = 0; i < (int) _vDimension; ++i) {
        std::vector<bool> binary;
        for (int j = 0; j < (int) _hDimension; ++j) {
            binary.push_back(0);
        }
        binaries.push_back(binary);
    }
    _highlights = binaries;
}

bool PlayBoard::move(Command command) {
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
        if (_letters.at(vIndex-1).at(hIndex) != ' ') { //letter before at the collumn
            if (_highlights.at(vIndex-1).at(hIndex) == 0) return false;
        }
    }

    _highlights.at(vIndex).at(hIndex) = 1;
    return true;
} 