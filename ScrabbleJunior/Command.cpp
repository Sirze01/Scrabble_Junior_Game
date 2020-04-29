#include "Command.h"
#include "../common/StringProcess.h"
#include "../common/Board.h"

Command::Command(std::string userInput) {
	_str = stripSpaces(userInput);
	if (isMove()) {
		_commandType = 1;
	}
	else {
		_str = lowerCase(_str);
		if (_str == "exchange") { // only executed when possible. implement later
			_commandType = 2;
		}
		else if (_str == "check hands") {
			_commandType = 3;
		}
		else if (_str == "check pool") {
			_commandType = 4;
		}
		else if (_str == "help") {
			_commandType = 5;
		}
		else {
			_commandType = -1;
		}
	}
}

int Command::getCommand() const {
	return _commandType;
}

bool Command::isMove() const {
	if (_str.size() != 4) return false;
	if (!(_str.at(0) >= 'A' && _str.at(0) <= 'Z')) return false;
	if (!(_str.at(1) >= 'a' && _str.at(0) <= 'z')) return false;
	if (_str.at(2) != ' ') return false;
	if (!(_str.at(3) >= 'A' && _str.at(3) <= 'Z')) return false;
	return true;
}

coord Command::getMovePos(Board board) const {
	if (!isMove()) return { -1,-1 };
	return board.getIndex(_str.substr(0, 2));
}

char Command::getMoveLetter() const {
	if (!isMove()) return '?';
	return _str.at(3);
}