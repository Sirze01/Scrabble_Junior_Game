#include "Board.h"
#include "../common/ConsoleSetup.h"

const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

void Board::defaultInit(size_t nLines, size_t nColumns) {
    _vDimension = nLines;
    _hDimension = nColumns;

    _letters.resize(_vDimension);
    _highlights.resize(_vDimension);
    _highlightColors.resize(_vDimension);

    for (size_t line = 0; line < _vDimension; ++line) {
        _letters.at(line).resize(_hDimension);
        _highlights.at(line).resize(_hDimension);
        _highlightColors.at(line).resize(_hDimension);

        for (size_t col = 0; col < _hDimension; ++col) {
            _letters.at(line).at(col) = SPACE;
            _highlights.at(line).at(col) = false;
            _highlightColors.at(line).at(col) = RED;
        }
    }
}

Board::Board(size_t nLines, size_t nColumns) {
    defaultInit(nLines, nColumns);
}

Board::Board(std::string filename) : Board() {
    int lineDim, colDim;
    std::string content;
	std::ifstream file;
	file.open(filename, std::ios::in);

	if (file.is_open()) {
        getline(file, content);

        lineDim = std::stoi(content.substr(0, content.find_first_of(SPACE)));
        content.erase(0, content.find_first_of(SPACE) + 3);
        colDim = std::stoi(content);

        defaultInit(lineDim, colDim);

        while(getline(file, content)){
            codedWord entry;
            entry.firstCoord = content.substr(0, 2);
            entry.orientation = content.at(3);
            entry.word = content.substr(5);
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
        defaultInit();
		std::cerr << "Cannot open file! Created default board." << std::endl;
	}
}

void Board::show() const { //Prototype function (needs styling)
	auto darkSpace = []() {outputBackColor(std::cout, DARK_GREY, SPACE); };

    std::cout << std::string(BOARD_TOP_PADDING,'\n') << LEFT_PADDING_STR;

	darkSpace();
	for (size_t i = 0; i < _hDimension; i++) {
		darkSpace(); outputBackColor(std::cout, DARK_GREY, alphabet.at(i));
	}
	//std::cout << std::endl;
	darkSpace(); darkSpace(); std::cout << std::endl;
	for (size_t i = 0; i < _vDimension; i++) {
		std::cout << LEFT_PADDING_STR;
		outputBackColor(std::cout, DARK_GREY,toupper(alphabet.at(i)));
		for (size_t j = 0; j < _hDimension; j++) {
			std::cout << SPACE;
			if (getHighlights().at(i).at(j)) {
				outputBackForeColor(std::cout, WHITE, _highlightColors.at(i).at(j), _letters.at(i).at(j));
			}
			else std::cout << _letters[i][j];
		}
		std::cout << " "; darkSpace(); std::cout << std::endl;
	}
	std::cout << LEFT_PADDING_STR;
	for (size_t i = 0; i <= 2 * _hDimension + 2; i++) darkSpace();

	//make room for card view
	int i = BOARD_MIN_DIM - _vDimension + BOARD_BOTTOM_PADDING; if (i < BOARD_BOTTOM_PADDING) i = BOARD_BOTTOM_PADDING;
	while (i--) std::cout << "\n";
}

coord Board::getIndex(const std::string &position) const {
	coord coordinates{};
	coordinates.vLine = alphabet.find(tolower(position.at(0)));
	coordinates.hCollumn = alphabet.find(position.at(1));
	return coordinates;
}

std::string Board::getPositionString(coord c) const {
    std::string str;
    str += static_cast<char>('A' + c.vLine);
    str += static_cast<char>('a' + c.hCollumn);
    return str;
}

bool Board::fileExport(std::string filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << _vDimension << 'x' << _hDimension << '\n';
        for (auto line : _words) {
            file << line.firstCoord << SPACE << line.orientation << SPACE << line.word << '\n';
        }
        return true;
    }
    std::cerr << "Could not write to file." << std::endl;
    return false;
}

bool Board::highlight(int color, int vIndex, int hIndex) {
	if (vIndex >= (int)_vDimension || hIndex >= (int)_hDimension) return false;
	if (_letters.at(vIndex).at(hIndex) == SPACE) return false;
	if (_highlights.at(vIndex).at(hIndex)) return false;

	_highlights.at(vIndex).at(hIndex) = 1;
	_highlightColors.at(vIndex).at(hIndex) = color;
	return true;
}

//Check for allways true conditions
void Board::highlightFinishedWord(int color, int vIndex, int hIndex) {
	std::vector<std::vector<int>> tempCol, tempLine;
	coord dim = getDimensions();
	bool successOnLine, successOnCol;

	for (int cof : {-1, 1}) {
		successOnCol = true; tempCol = _highlightColors;
		for (size_t line = vIndex + cof; line < dim.vLine; line+=cof) {
			if (_highlights.at(line).at(hIndex)) {
				tempCol.at(line).at(hIndex) = color;
			}
			else {
				if (_letters.at(line).at(hIndex) != SPACE) successOnCol = false;
				break;
			}
		}
		if (successOnCol) _highlightColors = tempCol;

		successOnLine = true; tempLine = _highlightColors;
		for (size_t col = hIndex + cof; col < dim.hCollumn; col+=cof) {
			if (_highlights.at(vIndex).at(col)) {
				tempLine.at(vIndex).at(col) = color;
			}
			else {
				if (_letters.at(vIndex).at(col) != SPACE) successOnLine = false;
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
		for (char c : v) if (c != SPACE) allChars.push_back(c);
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
bool Board::boardBounds(coord firstLetter, char orientation, int wordLen){ //ORIENTATION IS NOW A CHAR - MIND THIS
    return firstLetter.vLine < _vDimension
        && firstLetter.hCollumn < _hDimension
        && !(orientation == 'V' && firstLetter.vLine + wordLen >= _vDimension)
        && !(orientation == 'H' && firstLetter.hCollumn + wordLen >= _hDimension);
}

bool Board::goodIntersects(codedWord word) {
    bool valid = true;
    if(word.orientation == "V") {
        for(size_t i = 0; i < word.word.size(); i++){
            if (_letters[getIndex(word.firstCoord).vLine + i][getIndex(word.firstCoord).hCollumn] != SPACE)
                if(_letters[getIndex(word.firstCoord).vLine + i][getIndex(word.firstCoord).hCollumn] != word.word[i])
                    valid = false;
        }
    }
    if(word.orientation == "H") {
        for(size_t i = 0; i < word.word.size(); i++){
            if (_letters[getIndex(word.firstCoord).vLine][getIndex(word.firstCoord).hCollumn + i] != SPACE)
                if(_letters[getIndex(word.firstCoord).vLine][getIndex(word.firstCoord).hCollumn + i] != word.word[i])
                    valid = false;
        }
    }
    return valid;
}

void Board::lettersManip(coord c, char letter) { //setletter
    _letters[c.vLine][c.hCollumn] = letter;
}

void Board::addWord(codedWord word) {
    _words.push_back(word);
}

bool Board::wordSpaces(codedWord entry) { // isIsolated
    if(entry.orientation == "V") {

        for (size_t i = 0; i < entry.word.size(); i++) {
            if (i == 0 && (getIndex(entry.firstCoord).vLine - 1 <= _vDimension - 1)) {
                if (_letters[getIndex(entry.firstCoord).vLine + i - 1][getIndex(entry.firstCoord).hCollumn] != SPACE)
                    return false;
            }

            if (i == (entry.word.size() - 1) && (getIndex(entry.firstCoord).vLine + i + 1 <= _vDimension - 1)) {
                if (_letters[getIndex(entry.firstCoord).vLine + i + 1][getIndex(entry.firstCoord).hCollumn] != SPACE)
                    return false;
            }

            if (_letters[getIndex(entry.firstCoord).vLine + i][getIndex(entry.firstCoord).hCollumn] != entry.word[i]) {
                if (getIndex(entry.firstCoord).hCollumn + 1 <= _hDimension - 1) {
                    if (_letters[getIndex(entry.firstCoord).vLine + i][getIndex(entry.firstCoord).hCollumn + 1] != SPACE)
                        return false;
                }

                if (getIndex(entry.firstCoord).hCollumn - 1 <= _hDimension - 1) {
                    if (_letters[getIndex(entry.firstCoord).vLine + i][getIndex(entry.firstCoord).hCollumn - 1] != SPACE)
                        return false;

                }
            }
        }
    }

    if(entry.orientation == "H") {
        for (size_t i = 0; i < entry.word.size(); i++) {
            if (i == 0 && (getIndex(entry.firstCoord).hCollumn - 1 <= _hDimension - 1)) {
                if (_letters[getIndex(entry.firstCoord).vLine][getIndex(entry.firstCoord).hCollumn + i - 1] != SPACE)
                    return false;
            }

            if (i == (entry.word.size() - 1) && (getIndex(entry.firstCoord).hCollumn + i + 1 <= _hDimension - 1)) {
                if (_letters[getIndex(entry.firstCoord).vLine][getIndex(entry.firstCoord).hCollumn + i + 1] != SPACE)
                    return false;
            }

            if (_letters[getIndex(entry.firstCoord).vLine][getIndex(entry.firstCoord).hCollumn + i] != entry.word[i]) {
                if (getIndex(entry.firstCoord).vLine + 1 <= _vDimension - 1) {
                    if (_letters[getIndex(entry.firstCoord).vLine + 1][getIndex(entry.firstCoord).hCollumn + i] != SPACE)
                        return false;
                }

                if (getIndex(entry.firstCoord).vLine - 1 <= _vDimension - 1) {
                    if (_letters[getIndex(entry.firstCoord).vLine - 1][getIndex(entry.firstCoord).hCollumn + i] != SPACE)
                        return false;

                }
            }
        }
    }
    return true;
}