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
      std::cout << std::string(BOARD_TOP_PADDING,'\n') << std::string(BOARD_LEFT_PADDING, ' ');

	std::cout << " ";
	for (int i = 0; i < _hDimension; i++) {
		std::cout << " " << alphabet.at(i);
	}
	std::cout << std::endl;
	for (int i = 0; i < _vDimension; i++) {
		std::cout << std::string(BOARD_LEFT_PADDING, ' ');
		std::cout << std::string(1, (toupper(alphabet.at(i))));
		for (int j = 0; j < _hDimension; j++) {
			std::cout << ' ';
			if (getHighlights().at(i).at(j)) {
				print(WHITE, _highlightColors.at(i).at(j), _letters.at(i).at(j));
			}
			else std::cout << _letters[i][j];
		}
		std::cout << '\n';
	}
	std::cout << "\n\n";
}

coord Board::getIndex(std::string position) const {
	coord coordinates{};
	coordinates.vLine = alphabet.find(tolower(position.at(0)));
	coordinates.hCollumn = alphabet.find(position.at(1));
	return coordinates;
}

/*bool Board::fileExport(std::string filename) const {
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
}*/

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

	for (int line = vIndex - 1; line >= 0; line--) {
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
	coord dimensions = { _vDimension, _hDimension};
	return dimensions;
}


// Needs Testing - Reserved area
bool Board::boardBounds(coord firstLetter, std::string orientation, int wordLen){
    if((firstLetter.vLine > _vDimension) || (firstLetter.vLine < _vDimension))
        return false;
    if((firstLetter.hCollumn > _hDimension) || (firstLetter.hCollumn < _hDimension))
        return false;

    if(orientation == "V") {
        if ((firstLetter.vLine + wordLen > _vDimension) || (firstLetter.vLine + wordLen < _vDimension))
            return false;
    }

    if(orientation == "H") {
        if((firstLetter.hCollumn + wordLen > _hDimension) || (firstLetter.hCollumn + wordLen < _hDimension))
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