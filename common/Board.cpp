#include "Board.h"



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


Board::Board(std::string filename){
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

        codedWord entry;
        auto fillEntry = [&](std::string content){
            entry.firstCoord = getIndex(content.substr(0, 2));
            content.erase(0, 2);
            if (!content.empty())
                entry.orientation = content.at(1);
            content.erase(0, 2);
            if (!content.empty())
                entry.word = content.substr(1);
            return entry;
        };

        auto checkCoordCases = [](std::string letterCoords){
            return (isupper(letterCoords.at(0)) && islower(letterCoords.at(1)));
        };

        while(getline(file, content)) {
            if(content.size() > 2 && (checkCoordCases(content.substr(0, 2)) && boardBounds(fillEntry(content)))){
                _words.push_back(entry);
                coord index = entry.firstCoord;

                if(entry.orientation == 'H'){
                    for(size_t w = 0; w < entry.word.size(); w++){
                        _letters.at(index.vLine).at(index.hColumn + w) = entry.word.at(w);
                    }
                }
                else if (entry.orientation == 'V'){
                    for(size_t w = 0; w < entry.word.size(); w++){
                        _letters.at(index.vLine + w).at(index.hColumn) = entry.word.at(w);
                    }
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
		darkSpace(); outputBackColor(std::cout, DARK_GREY, _alphabet.at(i));
	}
	//std::cout << std::endl;
	darkSpace(); darkSpace(); std::cout << std::endl;
	for (size_t i = 0; i < _vDimension; i++) {
		std::cout << LEFT_PADDING_STR;
		outputBackColor(std::cout, DARK_GREY,toupper(_alphabet.at(i)));
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
	coordinates.vLine = _alphabet.find(tolower(position.at(0)));
	coordinates.hColumn = _alphabet.find(position.at(1));
	return coordinates;
}


std::string Board::getPositionString(coord c) const {
    std::string str;
    str += static_cast<char>('A' + c.vLine);
    str += static_cast<char>('a' + c.hColumn);
    return str;
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
		for (size_t col = hIndex + cof; col < dim.hColumn; col+=cof) {
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


bool Board::boardBounds(const codedWord &entry){
    if((entry.firstCoord.vLine > _vDimension) || (entry.firstCoord.vLine < 0))
        return false;
    if((entry.firstCoord.hColumn > _hDimension) || (entry.firstCoord.hColumn < 0))
        return false;
    if(entry.orientation == 'V') {
        if ((entry.firstCoord.vLine + entry.word.size() > _vDimension) || (entry.firstCoord.vLine + entry.word.size() < 0))
            return false;
    }

    if(entry.orientation == 'H') {
        if((entry.firstCoord.hColumn + entry.word.size() > _hDimension) || (entry.firstCoord.hColumn + entry.word.size() < 0))
            return false;
    }

    return true;
}


void Board::placeChar(coord inates, char character) {
    _letters[inates.vLine][inates.hColumn] = character;
}


void Board::addWord(codedWord word) {
    _words.push_back(word);
}


bool Board::wordSpaces(codedWord word) {
    if(word.orientation == 'V') {

        for (int i = 0; i < word.word.size(); i++) {
            if (i == 0 &&
                (word.firstCoord.vLine - 1 <= _vDimension - 1 && word.firstCoord.vLine - 1 >= 0)) {
                if (_letters.at(word.firstCoord.vLine + i - 1).at(word.firstCoord.hColumn) != ' ')
                    return false;
            }

            if (i == (word.word.size() - 1) && (word.firstCoord.vLine + i + 1 <= _vDimension - 1 &&
                                                word.firstCoord.vLine + i + 1 >= 0)) {
                if (_letters.at(word.firstCoord.vLine + i + 1).at(word.firstCoord.hColumn) != ' ')
                    return false;
            }

            if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn) != word.word[i]) {
                if (word.firstCoord.hColumn + 1 <= _hDimension - 1 &&
                    word.firstCoord.hColumn + 1 >= 0) {
                    if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn + 1) != ' ')
                        return false;
                }

                if (word.firstCoord.hColumn - 1 <= _hDimension - 1 &&
                    word.firstCoord.hColumn - 1 >= 0) {
                    if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn - 1) != ' ')
                        return false;

                }
            }
        }
    }


    if(word.orientation == 'H') {
        for (int i = 0; i < word.word.size(); i++) {
            if (i == 0 &&
                (word.firstCoord.hColumn - 1 <= _hDimension - 1 && word.firstCoord.hColumn - 1 >= 0)) {
                if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + i - 1) != ' ')
                    return false;
            }

            if (i == (word.word.size() - 1) && (word.firstCoord.hColumn + i + 1 <= _hDimension - 1 &&
                                                word.firstCoord.hColumn + i + 1 >= 0)) {
                if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + i + 1) != ' ')
                    return false;
            }

            if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + i) != word.word[i]) {
                if (word.firstCoord.vLine + 1 <= _vDimension - 1 &&
                    word.firstCoord.vLine + 1 >= 0) {
                    if (_letters.at(word.firstCoord.vLine + 1).at(word.firstCoord.hColumn + i) != ' ')
                        return false;
                }

                if (word.firstCoord.vLine - 1 <= _vDimension - 1 &&
                    word.firstCoord.vLine - 1 >= 0) {
                    if (_letters.at(word.firstCoord.vLine - 1).at(word.firstCoord.hColumn + i) != ' ')
                        return false;

                }
            }
        }
    }
    return true;
}


// Needs Testing - Reserved area :)
std::string Board::getAlphabet() const {
    return _alphabet;
}

std::vector<codedWord> Board::getWords() const {
    return _words;
}

void Board::removeWord(codedWord entry){
    for(size_t i = 0; i < _words.size(); i++){
        if (_words.at(i).word == entry.word){
            _words.erase(_words.begin() + i);
            break;
        }
    }
}

codedWord* Board::findWord(const std::string &word){
    for(auto &boardWord : _words){
         if(boardWord.word == word) return &boardWord;
    }
    return nullptr;
}

bool Board::wordExists(coord coordinates) const {
    for(size_t i = 0; i < _words.size(); i++){
        if (_words.at(i).firstCoord.vLine == coordinates.vLine && _words.at(i).firstCoord.hColumn == coordinates.hColumn){
            return true;
        }
    }
    return false;
}

bool Board::wordExists(std::string word) const {
    for(const auto &inBoard : _words){
        if (inBoard.word == word){
            return true;
        }
    }
    return false;
}

codedWord* Board::findWord(const coord &coordinates) {
    for(auto &boardWord : _words){
        if(boardWord.firstCoord == coordinates) return &boardWord;
    }
    return nullptr;
}

bool Board::checkIntersection(codedWord word) {
    bool valid = true;
    if(word.orientation == 'V') {
        for(size_t i = 0; i < word.word.size(); i++){
            if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn) != ' ') {
                if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn) != word.word.at(i))
                    valid = false;
            }
        }
    }
    if(word.orientation == 'H') {
        for (int i = 0; i < word.word.size(); i++) {
            if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + i) != ' ') {
                if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + i) != word.word.at(i))
                    valid = false;
            }
        }
    }
    return valid;
}

std::vector<coord> Board::intersectionsVector(codedWord word) {
    std::vector<coord> intersections;
    if(word.orientation == 'V') {
        for(size_t i = 0; i < word.word.size(); i++){
            if ((word.firstCoord.hColumn + 1) <= getDimensions().hColumn - 1 && (word.firstCoord.hColumn - 1) <= getDimensions().hColumn - 1) {
                if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn + 1) != ' ') {
                    intersections.push_back({word.firstCoord.vLine + i, word.firstCoord.hColumn});
                }
                if(_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn - 1) != ' ') {
                    intersections.push_back({word.firstCoord.vLine + i, word.firstCoord.hColumn});
                }
            }
        }
    }
    if(word.orientation == 'H') {
        for (int i = 0; i < word.word.size(); i++) {
            if ((word.firstCoord.vLine + 1) <= getDimensions().vLine - 1 && (word.firstCoord.vLine - 1) <= getDimensions().vLine - 1){
                if(_letters.at(word.firstCoord.vLine + 1).at(word.firstCoord.hColumn + i) != ' '){
                    intersections.push_back({word.firstCoord.vLine, word.firstCoord.hColumn + i});
                }
                if(_letters.at(word.firstCoord.vLine - 1).at(word.firstCoord.hColumn + i) != ' '){
                    intersections.push_back({word.firstCoord.vLine, word.firstCoord.hColumn + i});
                }
            }





        }
    }
    return intersections;
}

bool Board::fileExport(const std::string &filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << _vDimension << " x " << _hDimension << '\n';
        for (const auto &line : _words) {
            file << Board::getPositionString(line.firstCoord) << ' ' << line.orientation << ' ' << line.word << '\n';
        }

        file << std::string(2, '\n');
        file << std::string(BOARD_TOP_PADDING,'\n') << LEFT_PADDING_STR;
        file << ' ';
        for (int i = 0; i < _hDimension; i++) {
            file << " " << _alphabet.at(i);
        }
        file << std::endl;
        for (int i = 0; i < _vDimension; i++) {
            file << LEFT_PADDING_STR;
            file << std::string(1,toupper(_alphabet.at(i)));
            for (int j = 0; j < _hDimension; j++) {
                file << ' ';
                file << _letters[i][j];
            }
            file << " ";
            file << std::endl;
        }
        return true;
    }

    else {
        std::cerr << "Could not write to file." << std::endl;
        return false;
    }
}