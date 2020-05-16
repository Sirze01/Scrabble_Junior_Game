#include "Board.h"
#include <algorithm>

std::string Board::_alphabet = "abcdefghijklmnopqrstuvwxyz";

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
    if(entry.firstCoord.vLine > _vDimension)
        return false;
    if(entry.firstCoord.hColumn > _hDimension)
        return false;
    if(entry.orientation == 'V') {
        if (entry.firstCoord.vLine + entry.word.size() > _vDimension)
            return false;
    }

    if(entry.orientation == 'H') {
        if(entry.firstCoord.hColumn + entry.word.size() > _hDimension)
            return false;
    }

    return true;
}


void Board::placeChar(coord inates, char character) {
    _letters.at(inates.vLine).at(inates.hColumn) = character;
}


bool Board::addWord(codedWord word) {
    bool retValue = true;
    if(!checkIntersection(word)){
        std::cout << LEFT_PADDING_STR << stringWriter(100, "The word you're trying to add intersects with another in the wrong letter",
                             0) << std::endl;
        retValue = false;
    }
    if(!wordIsolation(word)){
        std::cout << LEFT_PADDING_STR << stringWriter(100, "The word you're trying to add doesn't fit in that space", 0) << std::endl;
        retValue = false;
    }
    if (retValue){
        if (word.orientation == 'V'){
            for(size_t i = 0; i < word.word.size(); i++){
                placeChar({word.firstCoord.vLine + i, word.firstCoord.hColumn}, word.word.at(i));
            }
        }
        else{
            for(size_t i = 0; i < word.word.size(); i++){
                placeChar({word.firstCoord.vLine, word.firstCoord.hColumn + i}, word.word[i]);
            }
        }
        _words.push_back(word);
        show();
    }
    return retValue;
}


bool Board::wordIsolation(codedWord word) const{
    if(word.orientation == 'V') {
        if (word.firstCoord.vLine - 1 < _vDimension){
            if (_letters.at(word.firstCoord.vLine - 1).at(word.firstCoord.hColumn) != ' ')
                return false;
        }
        if(word.firstCoord.vLine + word.word.size() + 1 < _vDimension){
            if (_letters.at(word.firstCoord.vLine + 1).at(word.firstCoord.hColumn) != ' ')
                return false;
        }
        for (size_t i = 0; i < word.word.size(); i++) {
            if(word.firstCoord.hColumn + 1 < _hDimension){
                if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn + 1) != ' ')
                    return false;
            }
            if(word.firstCoord.hColumn - 1 < _hDimension){
                if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn - 1) != ' ')
                    return false;
            }
        }
    }

    else{
        if (word.firstCoord.hColumn - 1 < _hDimension){
            if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn - 1) != ' ')
                return false;
        }
        if(word.firstCoord.hColumn + word.word.size() + 1 < _hDimension){
            if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + 1) != ' ')
                return false;
        }
        for (size_t i = 0; i < word.word.size(); i++) {
            if(word.firstCoord.vLine + 1 < _vDimension){
                if (_letters.at(word.firstCoord.vLine + 1).at(word.firstCoord.hColumn + i) != ' ')
                    return false;
            }
            if(word.firstCoord.vLine - 1 < _vDimension){
                if (_letters.at(word.firstCoord.vLine - 1).at(word.firstCoord.hColumn + i) != ' ')
                    return false;
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


bool Board::removeWord(std::string wordToRemove){
    if(wordExists(wordToRemove)){
        codedWord entryToRemove = *findWord(wordToRemove);
        std::vector<coord> intersections = getIntersectionsVector(entryToRemove);
        if (entryToRemove.orientation== 'V'){
            for(size_t i = 0; i < entryToRemove.word.size(); i++){
                coord current = {entryToRemove.firstCoord.vLine + i, entryToRemove.firstCoord.hColumn};
                if(intersections.empty()){
                    placeChar(current, ' ');
                }

                else{
                    if(std::find(intersections.begin(), intersections.end(), current) == intersections.end())
                        placeChar(current, ' ');
                }
            }
        }

        else{
            for(size_t i = 0; i < entryToRemove.word.size(); i++){
                coord current = {entryToRemove.firstCoord.vLine, entryToRemove.firstCoord.hColumn + i};
                if(intersections.empty()){
                    placeChar(current, ' ');
                }

                else{
                    if(std::find(intersections.begin(), intersections.end(), current) == intersections.end())
                        placeChar(current, ' ');
                }
            }
        }

        for(size_t i = 0; i < _words.size(); i++){
            if (_words.at(i).word == entryToRemove.word){
                _words.erase(_words.begin() + i);
                break;
            }
        }
        show();
        return true;
    }
    else{
        std::cout << std::string(BOARD_LEFT_PADDING, ' ') << "The word you trying to remove doesn't exist, try again"
                  << std::endl;
        return false;
    }


}


codedWord* Board::findWord(const std::string &word){
    for(auto &boardWord : _words){
         if(boardWord.word == word) return &boardWord;
    }
    return nullptr;
}


bool Board::wordExists(std::string word) const {
    for(const auto &inBoard : _words){
        if (inBoard.word == word){
            return true;
        }
    }
    return false;
}


bool Board::checkIntersection(codedWord word) const{
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
        for (size_t i = 0; i < word.word.size(); i++) {
            if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + i) != ' ') {
                if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + i) != word.word.at(i))
                    valid = false;
            }
        }
    }
    return valid;
}


std::vector<coord> Board::getIntersectionsVector(codedWord word) const {
    std::vector<coord> intersections;
    if(word.orientation == 'V') {
        for(size_t i = 0; i < word.word.size(); i++){
            if ((word.firstCoord.hColumn + 1) <= _hDimension - 1 && (word.firstCoord.hColumn - 1) <= _hDimension - 1) {
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
        for (size_t i = 0; i < word.word.size(); i++) {
            if ((word.firstCoord.vLine + 1) <= _vDimension - 1 && (word.firstCoord.vLine - 1) <= _vDimension - 1){
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
        for (size_t i = 0; i < _hDimension; i++) {
            file << " " << _alphabet.at(i);
        }
        file << std::endl;
        for (size_t i = 0; i < _vDimension; i++) {
            file << LEFT_PADDING_STR;
            file << std::string(1,toupper(_alphabet.at(i)));
            for (size_t j = 0; j < _hDimension; j++) {
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