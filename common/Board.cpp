#include "Board.h"
#include "../common/ConsoleSetup.h"

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

    _highlightColors.resize(_vDimension);
    for(auto &line : _highlightColors){
        line.resize(_hDimension);
        for(int i = 0; i < _hDimension; i++)
            line[i] = RED;
    }
}

Board::Board(int nLines, int nCollumns) {
	_vDimension = nLines;
	_hDimension = nCollumns;
	_letters.resize(_vDimension);
	for (auto& _letter : _letters) {
		_letter.resize(_hDimension);
		for (char& j : _letter) {
			j = ' ';
		}
	}

    _highlights.resize(_vDimension);
    for(auto &line : _highlights){
        line.resize(_hDimension);
        for(int i = 0; i < _hDimension; i++)
            line[i] = false;
    }

    _highlightColors.resize(_vDimension);
    for(auto &line : _highlightColors){
        line.resize(_hDimension);
        for(int i = 0; i < _hDimension; i++)
            line[i] = RED;
    }
}

Board::Board(std::string filename) {
	std::string line;
	std::ifstream file;
	file.open(filename, std::ios::in);

	if (file.is_open()) {
        getline(file, line);

        _vDimension = std::stoi(line.substr(0, line.find_first_of(' ')));
        line.erase(0, line.find_first_of(' ') + 3);
        _hDimension= std::stoi(line);

        _letters.resize(_vDimension);
        for (auto & _line : _letters){
            _line.resize(_hDimension);
            for(char & _letter : _line){
                _letter = ' ';
            }
        }

        while(getline(file, line) && line != "#####END_OF_BOARD#####"){
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
	else {
		int defaultSize = 20;
		_hDimension = defaultSize;
		_vDimension = defaultSize;
		_letters.resize(_vDimension);

		for (auto& _line : _letters) {
			_line.resize(_hDimension);
			for (char& _letter : _line) {
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

    _highlightColors.resize(_vDimension);
    for(auto &line : _highlightColors){
        line.resize(_hDimension);
        for(int i = 0; i < _hDimension; i++)
            line[i] = RED;
    }
}

void Board::show() const { //Prototype function (needs styling)
	auto darkSpace = []() {printBackColor(DARK_GREY, ' '); };

    std::cout << std::string(BOARD_TOP_PADDING,'\n') << LEFT_PADDING_STR;

	darkSpace();
	for (int i = 0; i < _hDimension; i++) {
		darkSpace(); printBackColor(DARK_GREY, alphabet.at(i));
		//std::cout << " " << alphabet.at(i);
	}
	//std::cout << std::endl;
	darkSpace(); darkSpace(); std::cout << std::endl;
	for (int i = 0; i < _vDimension; i++) {
		std::cout << LEFT_PADDING_STR;
		printBackColor(DARK_GREY,toupper(alphabet.at(i)));
		for (int j = 0; j < _hDimension; j++) {
			std::cout << ' ';
			if (getHighlights().at(i).at(j)) {
				print(WHITE, _highlightColors.at(i).at(j), _letters.at(i).at(j));
			}
			else std::cout << _letters[i][j];
		}
		std::cout << " "; darkSpace(); std::cout << std::endl;
	}
	std::cout << LEFT_PADDING_STR;
	for (int i = 0; i <= 2 * _hDimension + 2; i++) darkSpace();

	//make room for card view
	// PLEASE DO NOT TOUCH ON THIS!! IF YOU NEED TO CHANGE THESE SPACES, INCLUDE A DEFAULT ARGUMENT FOR IT
	//it means a lot to me. :) :)
	int i = BOARD_MIN_DIM - _vDimension + BOARD_BOTTOM_PADDING; if (i < BOARD_BOTTOM_PADDING) i = BOARD_BOTTOM_PADDING;
	while (i--) std::cout << "\n";
}

coord Board::getIndex(std::string position) const {
	coord coordinates{};
	coordinates.vLine = alphabet.find(tolower(position.at(0)));
	coordinates.hCollumn = alphabet.find(position.at(1));
	return coordinates;
}

bool Board::fileExport(std::string filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << _vDimension << " x " << _hDimension << '\n';
        for (auto line : _words) {
            file << line.firstCoord << ' ' << line.orientation << ' ' << line.word << '\n';
        }
        file << "#####END_OF_BOARD#####\n";

        file << std::string(2, '\n');
        file << std::string(BOARD_TOP_PADDING,'\n') << LEFT_PADDING_STR;
        for (int i = 0; i < _hDimension; i++) {
            file << "  " << alphabet.at(i);
        }
        file << std::endl;
        for (int i = 0; i < _vDimension; i++) {
            file << LEFT_PADDING_STR;
            file << std::string(1,toupper(alphabet.at(i)));
            for (int j = 0; j < _hDimension; j++) {
                file << ' ';
                file << _letters[i][j];
            }
            file << " ";
            file << std::endl;
        }

        return true;
    }
    else{
    std::cerr << "Could not write to file." << std::endl;
    return false;
    }
}

bool Board::highlight(int color, int vIndex, int hIndex) {
	if (vIndex >= (int)_vDimension || hIndex >= (int)_hDimension) return false;
	if (_letters.at(vIndex).at(hIndex) == ' ') return false;
	if (_highlights.at(vIndex).at(hIndex)) return false;

	_highlights.at(vIndex).at(hIndex) = 1;
	_highlightColors.at(vIndex).at(hIndex) = color;
	return true;
}

void Board::highlightFinishedWord(int color, int vIndex, int hIndex) {
	std::vector<std::vector<int>> tempCol, tempLine;
	coord dim = getDimensions();
	bool successOnLine, successOnCol;

	for (int cof : {-1, 1}) {
		successOnCol = true; tempCol = _highlightColors;
		for (int line = vIndex + cof; line >= 0 && line < dim.vLine; line+=cof) {
			if (_highlights.at(line).at(hIndex)) {
				tempCol.at(line).at(hIndex) = color;
			}
			else {
				if (_letters.at(line).at(hIndex) != ' ') successOnCol = false;
				break;
			}
		}
		if (successOnCol) _highlightColors = tempCol;

		successOnLine = true; tempLine = _highlightColors;
		for (int col = hIndex + cof; col >= 0 && col < dim.hCollumn; col+=cof) {
			if (_highlights.at(vIndex).at(col)) {
				tempLine.at(vIndex).at(col) = color;
			}
			else {
				if (_letters.at(vIndex).at(col) != ' ') successOnLine = false;
				break;
			}
		}
		if (successOnLine) _highlightColors = tempLine;
	}
}

std::vector<std::vector<char>> Board::getLetters() const {
	return _letters;
}

std::vector<char> Board::getNonEmptyChars() const {
	std::vector<char> allChars;
	for (std::vector<char> v : _letters) {
		for (char c : v) if (c != ' ') allChars.push_back(c);
	}
	return allChars;
}

std::vector<std::vector<bool>> Board::getHighlights() const {
	return _highlights;
}

coord Board::getDimensions() const {
	coord dimensions = { _vDimension, _hDimension};
	return dimensions;
}


// Needs Testing - Reserved area :)
bool Board::boardBounds(coord firstLetter, std::string orientation, int wordLen){
    if((firstLetter.vLine > _vDimension) || (firstLetter.vLine < 0))
        return false;
    if((firstLetter.hCollumn > _hDimension) || (firstLetter.hCollumn < 0))
        return false;

    if(orientation == "V") {
        if ((firstLetter.vLine + wordLen > _vDimension) || (firstLetter.vLine + wordLen < 0))
            return false;
    }

    if(orientation == "H") {
        if((firstLetter.hCollumn + wordLen > _hDimension) || (firstLetter.hCollumn + wordLen < 0))
            return false;
    }

    return true;
}

bool Board::goodIntersects(codedWord word) {
    bool valid = true;
    if(word.orientation == "V") {
        for(int i = 0; i < word.word.size(); i++){
            if (_letters[getIndex(word.firstCoord).vLine + i][getIndex(word.firstCoord).hCollumn] != ' ')
                if(_letters[getIndex(word.firstCoord).vLine + i][getIndex(word.firstCoord).hCollumn] != word.word[i])
                    valid = false;
        }
    }
    if(word.orientation == "H") {
        for(int i = 0; i < word.word.size(); i++){
            if (_letters[getIndex(word.firstCoord).vLine][getIndex(word.firstCoord).hCollumn + i] != ' ')
                if(_letters[getIndex(word.firstCoord).vLine][getIndex(word.firstCoord).hCollumn + i] != word.word[i])
                    valid = false;
        }
    }
    return valid;
}

void Board::lettersManip(coord inates, char letter) { //setletter
    _letters[inates.vLine][inates.hCollumn] = letter;
}

void Board::addWord(codedWord word) {
    _words.push_back(word);
}

bool Board::wordSpaces(codedWord word) {
    if(word.orientation == "V") {

        for (int i = 0; i < word.word.size(); i++) {
            if (i == 0 &&
                (getIndex(word.firstCoord).vLine - 1 <= _vDimension - 1 && getIndex(word.firstCoord).vLine - 1 >= 0)) {
                if (_letters[getIndex(word.firstCoord).vLine + i - 1][getIndex(word.firstCoord).hCollumn] != ' ')
                    return false;
            }

            if (i == (word.word.size() - 1) && (getIndex(word.firstCoord).vLine + i + 1 <= _vDimension - 1 &&
                                                getIndex(word.firstCoord).vLine + i + 1 >= 0)) {
                if (_letters[getIndex(word.firstCoord).vLine + i + 1][getIndex(word.firstCoord).hCollumn] != ' ')
                    return false;
            }

            if (_letters[getIndex(word.firstCoord).vLine + i][getIndex(word.firstCoord).hCollumn] != word.word[i]) {
                if (getIndex(word.firstCoord).hCollumn + 1 <= _hDimension - 1 &&
                    getIndex(word.firstCoord).hCollumn + 1 >= 0) {
                    if (_letters[getIndex(word.firstCoord).vLine + i][getIndex(word.firstCoord).hCollumn + 1] != ' ')
                        return false;
                }

                if (getIndex(word.firstCoord).hCollumn - 1 <= _hDimension - 1 &&
                    getIndex(word.firstCoord).hCollumn - 1 >= 0) {
                    if (_letters[getIndex(word.firstCoord).vLine + i][getIndex(word.firstCoord).hCollumn - 1] != ' ')
                        return false;

                }
            }
        }
    }

    if(word.orientation == "H") {
        for (int i = 0; i < word.word.size(); i++) {
            if (i == 0 &&
                (getIndex(word.firstCoord).hCollumn - 1 <= _hDimension - 1 && getIndex(word.firstCoord).hCollumn - 1 >= 0)) {
                if (_letters[getIndex(word.firstCoord).vLine][getIndex(word.firstCoord).hCollumn + i - 1] != ' ')
                    return false;
            }

            if (i == (word.word.size() - 1) && (getIndex(word.firstCoord).hCollumn + i + 1 <= _hDimension - 1 &&
                                                getIndex(word.firstCoord).hCollumn + i + 1 >= 0)) {
                if (_letters[getIndex(word.firstCoord).vLine][getIndex(word.firstCoord).hCollumn + i + 1] != ' ')
                    return false;
            }

            if (_letters[getIndex(word.firstCoord).vLine][getIndex(word.firstCoord).hCollumn + i] != word.word[i]) {
                if (getIndex(word.firstCoord).vLine + 1 <= _vDimension - 1 &&
                    getIndex(word.firstCoord).vLine + 1 >= 0) {
                    if (_letters[getIndex(word.firstCoord).vLine + 1][getIndex(word.firstCoord).hCollumn + i] != ' ')
                        return false;
                }

                if (getIndex(word.firstCoord).vLine - 1 <= _vDimension - 1 &&
                    getIndex(word.firstCoord).vLine - 1 >= 0) {
                    if (_letters[getIndex(word.firstCoord).vLine - 1][getIndex(word.firstCoord).hCollumn + i] != ' ')
                        return false;

                }
            }
        }
    }
    return true;
}