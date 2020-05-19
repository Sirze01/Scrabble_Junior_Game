#include "Board.h"

/**
 * Shared coode between both initializers
 * @param nLines - Number of lines in the board
 * @param nColumns - Number of columns in the board
 */
void Board::defaultInit(size_t nLines, size_t nColumns) {
	_nLines = nLines;
	_nCols = nColumns;

	_letters.resize(_nLines);
	_highlights.resize(_nLines);
	_highlightColors.resize(_nLines);


	for (size_t line = 0; line < _nLines; ++line) {
		_letters.at(line).resize(_nCols);
		_highlights.at(line).resize(_nCols);
		_highlightColors.at(line).resize(_nCols);
		for (size_t col = 0; col < _nCols; ++col) {
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
Board::Board(size_t nLines, size_t nColumns) : _nLines(BOARD_MIN_DIM), _nCols(BOARD_MIN_DIM) {
	defaultInit(nLines, nColumns);
}

/**
 * Constructor of boards from files, importing the tiles and words (codedWords)
 * @param filename - Text file to read from, encoded according to the project specification
 */
Board::Board(const std::string& fileName) : _nLines(BOARD_MIN_DIM), _nCols(BOARD_MIN_DIM) {
	int lineDim, colDim;
	std::string content;
	std::ifstream file;
	file.open(fileName, std::ios::in);

	if (file.is_open()) {
		getline(file, content);

		lineDim = std::stoi(content.substr(0, content.find_first_of(SPACE)));
		content.erase(0, content.find_first_of(SPACE) + 3);
		colDim = std::stoi(content);

		defaultInit(lineDim, colDim);

		Word entry = { {SIZE_MAX,SIZE_MAX}, '\0', std::string() };
		auto fillEntry = [&](std::string content) {
			entry.firstLetterPos = getIndex(content.substr(0, 2));
			content.erase(0, 2);
			if (!content.empty())
				entry.orientation = content.at(1);
			content.erase(0, 2);
			if (!content.empty())
				entry.str = content.substr(1);
			return entry;
		};

		auto checkCoordCases = [](std::string letterCoords) {
			return (std::isupper(letterCoords.at(0)) && std::islower(letterCoords.at(1)));
		};

		while (std::getline(file, content)) {
			if (content.size() > 2 && (checkCoordCases(content.substr(0, 2)) && boardBounds(fillEntry(content)))) {
				_words.push_back(entry);
				Coord index = entry.firstLetterPos;

				if (entry.orientation == 'H') {
					for (size_t w = 0; w < entry.str.size(); w++) {
						_letters.at(index.line).at(index.col + w) = entry.str.at(w);
					}
				}
				else if (entry.orientation == 'V') {
					for (size_t w = 0; w < entry.str.size(); w++) {
						_letters.at(index.line + w).at(index.col) = entry.str.at(w);
					}
				}
			}
		}

		file.close();
	}
	else {
		defaultInit();
		std::cerr << LEFT_PADDING_STR << "Could not open " << fileName << "! Created default board.\n";
	}
}

/**
 * Method to output the board to the console
 */
void Board::show() const { //Prototype function (needs styling)
	auto darkSpace = []() {Util::outputBackColor(std::cout, DARK_GREY, SPACE); };

	std::cout << std::string(BOARD_TOP_PADDING, '\n') << LEFT_PADDING_STR;

	darkSpace();

	for (size_t i = 0; i < _nCols; i++) {
		darkSpace(); Util::outputBackColor(std::cout, DARK_GREY, getAlpha(i, false));
	}

	darkSpace(); darkSpace(); std::cout << std::endl;
	for (size_t i = 0; i < _nLines; i++) {
		std::cout << LEFT_PADDING_STR;
		Util::outputBackColor(std::cout, DARK_GREY, getAlpha(i));
		for (size_t j = 0; j < _nCols; j++) {
			std::cout << SPACE;
			if (getHighlights().at(i).at(j)) {
				Util::outputBackColor(std::cout, _highlightColors.at(i).at(j), _letters.at(i).at(j));
			}
			else std::cout << _letters.at(i).at(j);
		}
		std::cout << SPACE; darkSpace(); std::cout << std::endl;
	}
	std::cout << LEFT_PADDING_STR;
	for (size_t i = 0; i <= 2 * _nCols + 2; i++) darkSpace();

	//make room for card view
	size_t i = BOARD_MIN_DIM - _nLines + BOARD_BOTTOM_PADDING;
	if (i < BOARD_BOTTOM_PADDING || i > BOARD_MIN_DIM) i = BOARD_BOTTOM_PADDING;
	while (i--) std::cout << "\n";
}

/**
 * Returns a struct Coord with the indexes indicated by the pair of letters in the parameters
 * @param position - Pair of letters to decode into indexes
 * @return - Decoded indexes
 */
Coord Board::getIndex(const std::string& position) {
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
		return static_cast<char>('A' + index);
	else
		return static_cast<char>('a' + index);
}

/**
 * Method get a string with the coordinates (displayed with letters) from a struct Coord
 * @param c - Coord to convert
 * @return - String with coords
 */
std::string Board::getPositionString(Coord c) {
	std::stringstream ss;
	ss << static_cast<char>('A' + c.line) << static_cast<char>('a' + c.col);
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
	if (vIndex >= _nLines || hIndex >= _nCols ||
		_letters.at(vIndex).at(hIndex) == SPACE || _highlights.at(vIndex).at(hIndex)) return false;

	_highlights.at(vIndex).at(hIndex) = true;
	_highlightColors.at(vIndex).at(hIndex) = color;
	return true;
}


/**
 * Method to highlight complete words on lines - "territory dominance" funtionality
 * @param color - Color code, defined in the consoleUtils.h file
 * @param vIndex - Index of the line of the tile to highlight
 * @param hIndex - Index of the column of the tile to highlight
 */
void Board::highlightWordOnLine(int color, size_t vIndex, size_t hIndex) {
	Coord dim = getDimensions();
	for (int direction : {-1, 1}) {
		for (size_t colDiff = hIndex + direction; colDiff < dim.col; colDiff += direction) {
			if (_letters.at(vIndex).at(colDiff) == SPACE) break;
			else _highlightColors.at(vIndex).at(colDiff) = color;
		}
	}
}

/**
 * Method to highlight complete words on columns - "territory dominance" funtionality
 * @param color - Color code, defined in the consoleUtils.h file
 * @param vIndex - Index of the line of the tile to highlight
 * @param hIndex - Index of the column of the tile to highlight
 */
void Board::highlightWordOnCol(int color, size_t vIndex, size_t hIndex) {
	Coord dim = getDimensions();
	for (int direction : {-1, 1}) {
		for (size_t lineDiff = vIndex + direction; lineDiff < dim.line; lineDiff += direction) {
			if (_letters.at(lineDiff).at(hIndex) == SPACE) break;
			else _highlightColors.at(lineDiff).at(hIndex) = color;
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
 * Method to get a struct Coord with the board dimensions
 * @return - Coord struct with board dimensions
 */
Coord Board::getDimensions() const {
	Coord dimensions = { _nLines, _nCols };
	return dimensions;
}


/**
 * Method to verify if a possible new entry to the board is in its bounds, necessary to cmdAdd
 * @param entry - codedWord with the word and fist positions of the new entry
 * @return - False if the word falls out of the board's boundaries, true otherwise
 */
bool Board::boardBounds(const Word& entry) const {
	return entry.firstLetterPos.line < _nLines&& entry.firstLetterPos.col < _nCols &&
		((entry.orientation == 'V' && entry.firstLetterPos.line + entry.str.size() <= _nLines)
			|| (entry.orientation == 'H' && entry.firstLetterPos.col + entry.str.size() <= _nCols));
}


/**
 * Method to edit the _letters private vector, changing the tile a given position, defined by the Coord struct given
 * @param c - Indexes of the tile to alter
 * @param character - New char to place in the index
 */
void Board::placeChar(Coord coord, char character) {
	_letters.at(coord.line).at(coord.col) = character;
}


/**
 * Method to add words to an open board, verifying intersections and word isolation
 * @param word - codedWord to add to the board
 * @param statusCode - Variable to control the external loop in case of fail
 * @return False if there's any bad intersection, if the word is not isolated from others, true otherwise
 */
bool Board::addWord(Word word, int& statusCode) {
	if (!checkIntersection(word)) {
		statusCode = -2;
		Util::stringWriter("The word you're trying does not intersect correctly with other words\n\n");
		return false;
	}
	else if (!wordIsolation(word)) {
		statusCode = -2;
		Util::stringWriter("You cannot create extra words or override old ones as a side effect of your addition\n\n");
		return false;
	}
	else {
		bool vertical = word.orientation == 'V';

		for (size_t i = 0; i < word.str.size(); i++) {
			if (vertical)
				placeChar({ word.firstLetterPos.line + i, word.firstLetterPos.col }, word.str.at(i));
			else
				placeChar({ word.firstLetterPos.line, word.firstLetterPos.col + i }, word.str[i]);
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
bool Board::wordIsolation(const Word& word) const {
	size_t initialLine = word.firstLetterPos.line, initialCol = word.firstLetterPos.col;
	bool vertical = word.orientation == 'V';
	Coord letterBeforePos, letterAfterPos;

	if (vertical) {
		letterBeforePos = { initialLine - 1 , initialCol };
		letterAfterPos = { initialLine + word.str.size() , initialCol };
	}
	else {
		letterBeforePos = { initialLine , initialCol - 1 };
		letterAfterPos = { initialLine, initialCol + word.str.size() };
	}

	//before and after must be spaces
	for (auto outside : { letterBeforePos, letterAfterPos }) {
		if (outside.line < _nLines && outside.col < _nCols)
			if (_letters.at(outside.line).at(outside.col) != SPACE)
				return false;
	}

	Coord current, neighbour;
	bool repeated = false;

	for (size_t letterInc = 0; letterInc < word.str.size(); letterInc++) {

		if (vertical) current = { initialLine + letterInc, initialCol };
		else current = { initialLine, initialCol + letterInc };

		//cannot append to current existing words, ie. no two repeated letters can be in board
		if (_letters.at(current.line).at(current.col) == word.str.at(letterInc)) {
			if (repeated) return false;
			else repeated = true;
		}
		else if (_letters.at(current.line).at(current.col) == SPACE) {
			repeated = false;
		}

		//cannot have letters on the sides
		for (auto diff : { -1,1 }) {
			if (vertical) neighbour = { current.line, current.col + diff };
			else neighbour = { current.line + diff, current.col };

			if (neighbour.line < _nLines && neighbour.col < _nCols) {
				if (_letters.at(current.line).at(current.col) == word.str.at(letterInc))
					continue; //same letters belonging to other words may have neighbours
				if (_letters.at(neighbour.line).at(neighbour.col) != SPACE)
					return false;
			}
		}
	}

	return true;
}


/**
 * Method to return the words being stores in the current board (private vector _words)
 * @return - Vector of codedWords with all the words stored in the board (private vector _words)
 */
std::vector<Word> Board::getWords() const {
	return _words;
}


/**
 * Funtion to remove words from the board, necessary to cmdRemove
 * @param wordToRemove - Word being removed
 * @param statusCode - Variable to control the external loop in case of fail
 * @return - False if the WordToRTemove doensn't belong to the board, true otherwise
 */
bool Board::removeWord(const std::string& wordToRemove, int& statusCode) {

	auto hasNeighbourIntersections = [&](const Word& word) {
		std::vector<Coord> intersections = getIntersectionsVector(word);
		for (const auto& i : intersections) {
			for (const auto& j : intersections) {
				if (i == j) continue;
				if (i.neighbour(j)) return true;
			}
		}
		return false;
	};

	if (!wordExists(wordToRemove)) {
		statusCode = -2;
		std::cout << LEFT_PADDING_STR << "The word you trying to remove doesn't exist, try again\n\n";
		return false;
	}
	else {
		Word entryToRemove = *findWord(wordToRemove);

		if (hasNeighbourIntersections(entryToRemove)) {
			statusCode = -2;
			std::cout << LEFT_PADDING_STR << "Removing that word would create other words as a side effect. Try another!\n\n";
			return false;
		}

		std::vector<Coord> intersections = getIntersectionsVector(entryToRemove);
		bool vertical = entryToRemove.orientation == 'V';

		//remove letters
		Coord current;
		for (size_t letterInc = 0; letterInc < entryToRemove.str.size(); letterInc++) {
			if (vertical)
				current = { entryToRemove.firstLetterPos.line + letterInc, entryToRemove.firstLetterPos.col };
			else
				current = { entryToRemove.firstLetterPos.line, entryToRemove.firstLetterPos.col + letterInc };

			if (std::find(intersections.begin(), intersections.end(), current) == intersections.end()) {
				placeChar(current, SPACE); //remove non-intersected letter
			}
		}

		//remove word from board data
		for (size_t i = 0; i < _words.size(); ++i) {
			if (_words.at(i).str != entryToRemove.str)
				continue;
			_words.erase(_words.begin() + i);
		}

		show(); //update board
		return true;
	}
}


/**
 * Method to get a pointer to the word stored in board
 * @param word - Word to find in the vector _words
 * @return - Pointer to the element of _words (element changed by other functions)
 */
Word* Board::findWord(const std::string& word) {
	for (auto& boardWord : _words) {
		if (boardWord.str == word) return &boardWord;
	}
	return nullptr;
}

/**
 * Method to verify if a word is already stored in board
 * @param word - Word to find in teh vector _words
 * @return True if the word exists, false otherwise
 */
bool Board::wordExists(const std::string& word) {
	return findWord(word) != nullptr;
}


/**
 * Method to check if the intersections when adding a word are made correctly
 * @param word - Word being added
 * @return - False if an intersections fails, true otherwise
 */
bool Board::checkIntersection(const Word& word) const {
	size_t initialLine = word.firstLetterPos.line, initialCol = word.firstLetterPos.col;
	char current; bool vertical = word.orientation == 'V';

	for (size_t i = 0; i < word.str.size(); i++) {
		if (vertical) {
			if (initialLine + i >= _nLines) continue;
			current = _letters.at(initialLine + i).at(initialCol);
		}
		else {
			if (initialCol + i >= _nCols) continue;
			current = _letters.at(initialLine).at(initialCol + i);
		}

		if (current != SPACE && current != word.str.at(i)) return false;
	}

	return true;
}


/**
 * Method to add all the intersections of a word to a vector, used to correctly remove a word
 * @param word - Word to remove
 * @return - Vector containing Coord structs with the intersection indexes
 */
std::vector<Coord> Board::getIntersectionsVector(const Word& word) const {
	std::vector<Coord> intersections;
	size_t initialLine = word.firstLetterPos.line, initialCol = word.firstLetterPos.col;
	bool vertical = word.orientation == 'V';

	for (size_t i = 0; i < word.str.size(); i++) {
		for (int cof : {-1, 1}) {
			if (vertical) {
				if (initialLine + i >= _nLines || initialCol + cof >= _nCols)
					continue;
				if (_letters.at(initialLine + i).at(initialCol + cof) != SPACE)
					intersections.push_back({ initialLine + i, initialCol });
			}
			else {
				if (initialLine + cof >= _nLines || word.firstLetterPos.col + i >= _nCols)
					continue;
				if (_letters.at(initialLine + cof).at(initialCol + i) != SPACE)
					intersections.push_back({ initialLine, initialCol + i });
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
		file << _nLines << " x " << _nCols << '\n';
		for (const auto& line : _words) {
			file << Board::getPositionString(line.firstLetterPos) << SPACE << line.orientation << SPACE << line.str << '\n';
		}

		file << "\n\n" << std::string(BOARD_TOP_PADDING, '\n') << LEFT_PADDING_STR << SPACE;
		for (size_t i = 0; i < _nCols; i++) {
			file << SPACE << getAlpha(i, false);
		}
		file << std::endl;
		for (size_t i = 0; i < _nLines; i++) {
			file << LEFT_PADDING_STR << getAlpha(i);
			for (size_t j = 0; j < _nCols; j++) {
				file << SPACE << _letters.at(i).at(j);
			}
			file << SPACE << std::endl;
		}
		return true;
	}

	else {
		std::cerr << LEFT_PADDING_STR << "Could not write to file.\n\n";
		return false;
	}
}