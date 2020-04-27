#include "Command.h"
#include "stringProcess.h"
#include "Board.h"

Command::Command(std::string userInput) {
	_str = stripSpaces(userInput);
	if (isMove()) {
		_commandType = 1;
		saveMove();
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

void Command::saveMove() {
	_move = { _str.substr(0,1), _str.substr(3) };
}

std::vector<std::string> Command::getMove() const {
	return _move;
}