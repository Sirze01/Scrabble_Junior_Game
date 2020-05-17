#include "Board.h"

/**
 * Shared coode between both initializers
 * @param nLines - Number of lines in the board
 * @param nColumns - Number of columns in the board
 */
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


/**
 * Constructor of empty boards of given dimensions (Height x Width)
 * @param nLines - Number of lines in the board
 * @param nColumns - Number of columns in the board
 */
Board::Board(size_t nLines, size_t nColumns) : _vDimension(BOARD_MIN_DIM), _hDimension(BOARD_MIN_DIM) {
	defaultInit(nLines, nColumns);
}


/**
 * Constructor of boards from files, importing the tiles and words (codedWords)
 * @param filename - Text file to read from, encoded according to the project specification
 */
Board::Board(const std::string& filename) : _vDimension(BOARD_MIN_DIM), _hDimension(BOARD_MIN_DIM) {
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

		codedWord entry = { {SIZE_MAX,SIZE_MAX}, '\0', std::string() };
		auto fillEntry = [&](std::string content) {
			entry.firstCoord = getIndex(content.substr(0, 2));
			content.erase(0, 2);
			if (!content.empty())
				entry.orientation = content.at(1);
			content.erase(0, 2);
			if (!content.empty())
				entry.word = content.substr(1);
			return entry;
		};

		auto checkCoordCases = [](std::string letterCoords) {
			return (isupper(letterCoords.at(0)) && islower(letterCoords.at(1)));
		};

		while (getline(file, content)) {
			if (content.size() > 2 && (checkCoordCases(content.substr(0, 2)) && boardBounds(fillEntry(content)))) {
				_words.push_back(entry);
				coord index = entry.firstCoord;

				if (entry.orientation == 'H') {
					for (size_t w = 0; w < entry.word.size(); w++) {
						_letters.at(index.vLine).at(index.hColumn + w) = entry.word.at(w);
					}
				}
				else if (entry.orientation == 'V') {
					for (size_t w = 0; w < entry.word.size(); w++) {
						_letters.at(index.vLine + w).at(index.hColumn) = entry.word.at(w);
					}
				}
			}
		}

		file.close();
	}
	else {
		defaultInit();
		std::cerr << LEFT_PADDING_STR << "Cannot open file! Created default board.\n";
	}
}


/**
 * Method to output the board to the console
 */
void Board::show() const { //Prototype function (needs styling)
	auto darkSpace = []() {Util::outputBackColor(std::cout, DARK_GREY, SPACE); };

	std::cout << std::string(BOARD_TOP_PADDING, '\n') << LEFT_PADDING_STR;

	darkSpace();

	for (size_t i = 0; i < _hDimension; i++) {
		darkSpace(); Util::outputBackColor(std::cout, DARK_GREY, getAlpha(i, false));
	}

	darkSpace(); darkSpace(); std::cout << std::endl;
	for (size_t i = 0; i < _vDimension; i++) {
		std::cout << LEFT_PADDING_STR;
		Util::outputBackColor(std::cout, DARK_GREY, getAlpha(i));
		for (size_t j = 0; j < _hDimension; j++) {
			std::cout << SPACE;
			if (getHighlights().at(i).at(j)) {
				Util::outputBackColor(std::cout, _highlightColors.at(i).at(j), _letters.at(i).at(j));
			}
			else std::cout << _letters.at(i).at(j);
		}
		std::cout << SPACE; darkSpace(); std::cout << std::endl;
	}
	std::cout << LEFT_PADDING_STR;
	for (size_t i = 0; i <= 2 * _hDimension + 2; i++) darkSpace();

	//make room for card view
	size_t i = BOARD_MIN_DIM - _vDimension + BOARD_BOTTOM_PADDING;
	if (i < BOARD_BOTTOM_PADDING || i > BOARD_MIN_DIM) i = BOARD_BOTTOM_PADDING;
	while (i--) std::cout << "\n";
}


/**
 * Returns a struct coord with the indexes indicated by the pair of letters in the parameters
 * @param position - Pair of letters to decode into indexes
 * @return - Decoded indexes
 */
coord Board::getIndex(const std::string& position) {
	return { getIndex(static_cast<char>(std::toupper(position.at(0)))),
		 getIndex(static_cast<char>(std::toupper(position.at(1)))) };
}


/**
 * Method to convert a letter in its respective index
 * @param letter - Letter to convert
 * @return - Index
 */
size_t Board::getIndex(char letter) {
	return letter - 'A';
}


/**
 * Convert a letter with a given index to its correspondent char,
 * @param index - Index of the letter (0 - 26)
 * @param uppercase - Indicates if the desired letter should be upper (true) or lowerCase (False)
 * @return - Char with the desired letter
 */
char Board::getAlpha(size_t index, bool uppercase) {
	if (uppercase)
		return  static_cast<char>('A' + index);
	else
		return  static_cast<char>('a' + index);
}


/**
 * Method get a string with the coordinates (displayed with letters) from a struct coord
 * @param c - Coord to convert
 * @return - String with coords
 */
std::string Board::getPositionString(coord c) {
	std::stringstream ss;
	ss << static_cast<char>('A' + c.vLine) << static_cast<char>('a' + c.hColumn);
	return ss.str();
}

/**
 * Method to highlight a letter tile, indicating that was chosen by a player
 * @param color - Color code, defined in the consoleUtils.h file
 * @param vIndex - Index of the line of the tile to highlight
 * @param hIndex - Index of the column of the tile to highlight
 * @return - Returns true if the highlight occurs successfully, false otherwise
 */
bool Board::highlight(int color, size_t vIndex, size_t hIndex) {
	if (vIndex >= _vDimension || hIndex >= _hDimension) return false;
	if (_letters.at(vIndex).at(hIndex) == SPACE) return false;
	if (_highlights.at(vIndex).at(hIndex)) return false;

	_highlights.at(vIndex).at(hIndex) = true;
	_highlightColors.at(vIndex).at(hIndex) = color;
	return true;
}


/**
 * Method to highlight complete words on lines, part of the territory domnance funtionality
 * @param color - Color code, defined in the consoleUtils.h file
 * @param vIndex - Index of the line of the tile to highlight
 * @param hIndex - Index of the column of the tile to highlight
 */
void Board::highlightWordOnLine(int color, size_t vIndex, size_t hIndex) {
	coord dim = getDimensions();
	for (int cof : {-1, 1}) {
		for (size_t i = hIndex + cof; i < dim.hColumn; i += cof) {
			if (_letters.at(vIndex).at(i) == SPACE) break;
			else _highlightColors.at(vIndex).at(i) = color;
		}
	}
}

/**
 * Method to highlight complete words on columns, part of the territory domnance funtionality
 * @param color - Color code, defined in the consoleUtils.h file
 * @param vIndex - Index of the line of the tile to highlight
 * @param hIndex - Index of the column of the tile to highlight
 */
void Board::highlightWordOnCol(int color, size_t vIndex, size_t hIndex) {
	coord dim = getDimensions();
	for (int cof : {-1, 1}) {
		for (size_t i = vIndex + cof; i < dim.vLine; i += cof) {
			if (_letters.at(i).at(hIndex) == SPACE) break;
			else _highlightColors.at(i).at(hIndex) = color;
		}
	}
}

/**
 * Method to return the _letters private vector, containing all board tiles
 * @return - _letters vector, with all board tiles
 */
std::vector<std::vector<char>> Board::getLetters() const {
	return _letters;
}

/**
 * Method to return all the letters from the _letters private vector, which contains all board tiles
 * @return -  vector, with all the board's letters
 */
std::vector<char> Board::getNonEmptyChars() const {
	std::vector<char> allChars;
	for (const std::vector<char>& v : _letters) {
		for (const char& c : v) if (c != SPACE) allChars.push_back(c);
	}
	return allChars;
}


/**
 * Method to return the private vector _highlights, which indicates the highlighted tiles
 * @return -  vector _highlights
 */
std::vector<std::vector<bool>> Board::getHighlights() const {
	return _highlights;
}

/**
 * Method to get a struct coord with the board dimensions
 * @return - coord struct with board dimensions
 */
coord Board::getDimensions() const {
	coord dimensions = { _vDimension, _hDimension };
	return dimensions;
}


/**
 * Method to verify if a possible new entry to the board is in its bounds, necessary to cmdAdd
 * @param entry - codedWord with the word and fist positions of the new entry
 * @return - False if the word falls out of the board's boundaries, true otherwise
 */
bool Board::boardBounds(const codedWord& entry) const {
	if (entry.firstCoord.vLine >= _vDimension || entry.firstCoord.hColumn >= _hDimension)
		return false;

	if (entry.orientation == 'V' && entry.firstCoord.vLine + entry.word.size() > _vDimension)
		return false;

	if (entry.orientation == 'H' && entry.firstCoord.hColumn + entry.word.size() > _hDimension)
		return false;

	return true;
}


/**
 * Method to edit the _letters private vector, changing the tile a given position, defined bu the coord struct given
 * @param c - Indexes of the tile to alter
 * @param character - New char to place in the index
 */
void Board::placeChar(coord c, char character) {
	_letters.at(c.vLine).at(c.hColumn) = character;
}


/**
 * Method to add words to an open board, verifying intersections and word Isolation
 * @param word - codedWord to add to the board
 * @param statusCode - Variable to control the external loop in case of fail
 * @return False if there's any bad intersection, if the word is not isolated from others, true otherwise
 */
bool Board::addWord(codedWord word, int& statusCode) {
	if (!checkIntersection(word)) {
		statusCode = -2;
		Util::stringWriter("The word you're trying to add intersects with another in the wrong letter\n\n");
		return false;
	}
	else if (!wordIsolation(word)) {
		statusCode = -2;
		Util::stringWriter("The word you're trying to add doesn't fit in that space\n\n");
		return false;
	}
	else {
		if (word.orientation == 'V') {
			for (size_t i = 0; i < word.word.size(); i++) {
				placeChar({ word.firstCoord.vLine + i, word.firstCoord.hColumn }, word.word.at(i));
			}
		}
		else {
			for (size_t i = 0; i < word.word.size(); i++) {
				placeChar({ word.firstCoord.vLine, word.firstCoord.hColumn + i }, word.word[i]);
			}
		}
		_words.push_back(word);
		show();
	}
	return true;
}


/**
 * Method to check if the word that it's being added is isolated from others (except intersections)
 * @param word - codedWord to test
 * @return - False if the word isn't isolated, true otherwise
 */
bool Board::wordIsolation(const codedWord& word) const {
	if (word.orientation == 'V') {

	    if (word.firstCoord.vLine - 1 < _vDimension) {
	        if (_letters.at(word.firstCoord.vLine - 1).at(word.firstCoord.hColumn) != SPACE)
	            return false;
	    }
        for (int cof : {-1, 1}) {
			for (size_t i = 0; i < word.word.size(); i++) {
				if (word.firstCoord.hColumn + cof < _hDimension) {
					if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn) != word.word.at(i)) {
						if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn + cof) != SPACE)
							return false;
					}
				}
			}
		}
        if (word.firstCoord.vLine + 1 < _vDimension) {
            if (_letters.at(word.firstCoord.vLine+ word.word.size() + 1).at(word.firstCoord.hColumn) != SPACE)
                return false;
        }
	}

	else {

	    if (word.firstCoord.hColumn - 1 < _hDimension) {
	        if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn - 1) != SPACE)
	            return false;
	    }
        for (int cof : {-1, 1}) {
			for (size_t i = 0; i < word.word.size(); i++) {
				if (word.firstCoord.vLine + cof < _vDimension) {
					if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + i) != word.word.at(i)) {
						if (_letters.at(word.firstCoord.vLine + cof).at(word.firstCoord.hColumn + i) != SPACE)
							return false;
					}
				}
			}
		}
        if (word.firstCoord.hColumn + 1 < _hDimension) {
            if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + word.word.size() + 1) != SPACE)
                return false;
        }
	}

	return true;
}


/**
 * Method to return the words being stores in the current board (private vector _words)
 * @return - Vector of codedWords with all the words stored in the board (private vector _words)
 */
std::vector<codedWord> Board::getWords() const {
	return _words;
}


/**
 * Funtion to remove words from the board, necessary to cmdRemove
 * @param wordToRemove - Word being removed
 * @param statusCode - Variable to control the external loop in case of fail
 * @return - False if the WordToRTemove doensn't belong to the board, true otherwise
 */
bool Board::removeWord(const std::string& wordToRemove, int& statusCode) {
	if (wordExists(wordToRemove)) {
		codedWord entryToRemove = *findWord(wordToRemove);
		std::vector<coord> intersections = getIntersectionsVector(entryToRemove);
		if (entryToRemove.orientation == 'V') {
			for (size_t i = 0; i < entryToRemove.word.size(); i++) {
				coord current = { entryToRemove.firstCoord.vLine + i, entryToRemove.firstCoord.hColumn };
				if (intersections.empty()) {
					placeChar(current, SPACE);
				}

				else {
					if (std::find(intersections.begin(), intersections.end(), current) == intersections.end())
						placeChar(current, SPACE);
				}
			}
		}

		else {
			for (size_t i = 0; i < entryToRemove.word.size(); i++) {
				coord current = { entryToRemove.firstCoord.vLine, entryToRemove.firstCoord.hColumn + i };
				if (intersections.empty()) {
					placeChar(current, SPACE);
				}

				else {
					if (std::find(intersections.begin(), intersections.end(), current) == intersections.end())
						placeChar(current, SPACE);
				}
			}
		}

		for (size_t i = 0; i < _words.size(); i++) {
			if (_words.at(i).word == entryToRemove.word) {
				_words.erase(_words.begin() + i);
				break;
			}
		}
		show();
		return true;
	}
	else {
		statusCode = -2;
		std::cout << LEFT_PADDING_STR << "The word you trying to remove doesn't exist, try again\n";
		return false;
	}
}


/**
 * Method to get a pointer to the 'word' in the private vector of codedWords _words
 * @param word - Word to find in the vector _words
 * @return - Pointer to the element of _words (element altered by other functions)
 */
codedWord* Board::findWord(const std::string& word) {
	for (auto& boardWord : _words) {
		if (boardWord.word == word) return &boardWord;
	}
	return nullptr;
}


/**
 * Method to verify if a word belongs to the private vector codedWords _words, used to prevent a call of findWord, with no correspondant
 * @param word - Word to find in teh vector _words
 * @return True if the word exists, false otherwise
 */
bool Board::wordExists(const std::string& word) const {
	for (const auto& inBoard : _words) {
		if (inBoard.word == word) {
			return true;
		}
	}
	return false;
}


/**
 * Method to check if the intersections when adding a word are made correctly
 * @param word - Word being added
 * @return - False if an intersections fails, true otherwise
 */
bool Board::checkIntersection(const codedWord& word) const {
	if (word.orientation == 'V') {
		for (size_t i = 0; i < word.word.size(); i++) {
			if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn) != SPACE) {
				if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn) != word.word.at(i))
					return false;
			}
		}
	}
	if (word.orientation == 'H') {
		for (size_t i = 0; i < word.word.size(); i++) {
			if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + i) != SPACE) {
				if (_letters.at(word.firstCoord.vLine).at(word.firstCoord.hColumn + i) != word.word.at(i))
					return false;
			}
		}
	}
	return true;
}


/**
 * Method to add all the intersections of a word to a vector, used to correctly remove a word
 * @param word - Word to remove
 * @return - Vector containing coord structs with the intersection indexes
 */
std::vector<coord> Board::getIntersectionsVector(const codedWord& word) const {
	std::vector<coord> intersections;
	if (word.orientation == 'V') {
		for (size_t i = 0; i < word.word.size(); i++) {
			if ((word.firstCoord.hColumn + 1) <= _hDimension - 1 && (word.firstCoord.hColumn - 1) <= _hDimension - 1) {
				if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn + 1) != SPACE) {
					intersections.push_back({ word.firstCoord.vLine + i, word.firstCoord.hColumn });
				}
				if (_letters.at(word.firstCoord.vLine + i).at(word.firstCoord.hColumn - 1) != SPACE) {
					intersections.push_back({ word.firstCoord.vLine + i, word.firstCoord.hColumn });
				}
			}
		}
	}
	if (word.orientation == 'H') {
		for (size_t i = 0; i < word.word.size(); i++) {
			if ((word.firstCoord.vLine + 1) <= _vDimension - 1 && (word.firstCoord.vLine - 1) <= _vDimension - 1) {
				if (_letters.at(word.firstCoord.vLine + 1).at(word.firstCoord.hColumn + i) != SPACE) {
					intersections.push_back({ word.firstCoord.vLine, word.firstCoord.hColumn + i });
				}
				if (_letters.at(word.firstCoord.vLine - 1).at(word.firstCoord.hColumn + i) != SPACE) {
					intersections.push_back({ word.firstCoord.vLine, word.firstCoord.hColumn + i });
				}
			}
		}
	}
	return intersections;
}


/**
 * Method to export a board to an external text file
 * @param filename - Name of the file
 * @return - True if the write occurs successfully, false if it isn't possible to write to the file
 */
bool Board::fileExport(const std::string& filename) const {
	std::ofstream file(filename);
	if (file.is_open()) {
		file << _vDimension << " x " << _hDimension << '\n';
		for (const auto& line : _words) {
			file << Board::getPositionString(line.firstCoord) << SPACE << line.orientation << SPACE << line.word << '\n';
		}

		file << std::string(2, '\n');
		file << std::string(BOARD_TOP_PADDING, '\n') << LEFT_PADDING_STR;
		file << SPACE;
		for (size_t i = 0; i < _hDimension; i++) {
			file << SPACE << getAlpha(i, false);
		}
		file << std::endl;
		for (size_t i = 0; i < _vDimension; i++) {
			file << LEFT_PADDING_STR << getAlpha(i);
			for (size_t j = 0; j < _hDimension; j++) {
				file << SPACE << _letters.at(i).at(j);
			}
			file << SPACE << std::endl;
		}
		return true;
	}

	else {
		std::cerr << "Could not write to file." << std::endl;
		return false;
	}
}