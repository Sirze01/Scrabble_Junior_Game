#include "Board.h"
#include "../common/ConsoleSetup.h"

const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

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
	//fill highlights with 0 and RED color
	std::vector<std::vector<bool>> binaries;
	std::vector<std::vector<int>> colors;
	for (int i = 0; i < (int)_vDimension; ++i) {
		std::vector<bool> binary; std::vector<int> color;
		for (int j = 0; j < (int)_hDimension; ++j) {
			binary.push_back(0);
			color.push_back(RED);
		}
		binaries.push_back(binary);
		colors.push_back(color);
	}
	_highlights = binaries;
	_highlightColors = colors;
}

Board::Board(std::string filename) {
	std::string line;
	std::ifstream file;
	file.open(filename, std::ios::in);

	if (file.is_open()) {
		getline(file, line);
		_hDimension = std::stoi(line.substr(0, 2));
		_vDimension = std::stoi(line.substr(5));
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

	//fill highlights with 0 and RED color
	std::vector<std::vector<bool>> binaries;
	std::vector<std::vector<int>> colors;
	for (int i = 0; i < (int)_vDimension; ++i) {
		std::vector<bool> binary; std::vector<int> color;
		for (int j = 0; j < (int)_hDimension; ++j) {
			binary.push_back(0);
			color.push_back(RED);
		}
		binaries.push_back(binary);
		colors.push_back(color);
	}
	_highlights = binaries;
	_highlightColors = colors;
}

void Board::show() const { //Prototype function (needs styling)
	std::cout << " ";
	for (int i = 0; i < _hDimension; i++) {
		std::cout << " " << alphabet.at(i);
	}
	std::cout << std::endl;
	for (int i = 0; i < _vDimension; i++) {
		std::cout << std::string(1, (toupper(alphabet.at(i))));
		for (int j = 0; j < _hDimension; j++) {
			std::cout << ' ';
			if (getHighlights().at(i).at(j)) {
				printBackColor(_highlightColors.at(i).at(j), _letters.at(i).at(j));
			}
			else std::cout << _letters[i][j];
		}
		std::cout << '\n';
	}
	std::cout << "\n";
}

coord Board::getIndex(std::string position) const {
	coord coordinates{};
	coordinates.vLine = alphabet.find(tolower(position.at(0)));
	coordinates.hCollumn = alphabet.find(position.at(1));
	return coordinates;
}

bool Board::fileExport(std::string filename) const {
	std::string line;
	std::ofstream file(filename);
	if (file.is_open()) {
		file << _hDimension << 'x' << _vDimension << '\n';
		for (auto line : _words) {
			file << line << '\n';
		}
		return true;
	}
	std::cerr << "Could not write to file." << std::endl;
	return false;
}

Board::Board() {
	_hDimension = 20;
	_vDimension = 20;
	_letters.resize(_vDimension);
	for (size_t i = 0; i < _letters.size(); i++) {
		_letters[i].resize(_hDimension);
		for (size_t j = 0; j < _letters[i].size(); j++) {
			_letters[i][j] = ' ';
		}
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
	std::vector<std::vector<int>> tempCol = _highlightColors;
	std::vector<std::vector<int>> tempLine = _highlightColors;
	bool successOnLine = true, successOnCol = true;

	for (int line = vIndex-1; line >= 0; line--) {
		if (_highlights.at(line).at(hIndex)) {
			tempCol.at(line).at(hIndex) = color;
		}
		else {
			if (_letters.at(line).at(hIndex) != ' ') successOnCol = false;
			break;
		}
	}
	if (successOnCol) _highlightColors = tempCol;

	for (int col = hIndex - 1; col >= 0; col--) {
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