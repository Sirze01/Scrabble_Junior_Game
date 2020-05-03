#include "Command.h"
#include "../common/StringProcess.h"
#include "../common/Board.h"
#include <string>

Command::Command(std::string userInput) {
	_str = stripSpecialChars(userInput);
	_str = lowerCase(_str);
	_str = stripCommandBloat(_str);
	_str = stripSpaces(_str);
	std::cout << "received command: " << "'" << _str << "'\n";
}

bool Command::isMove() const {
	if (_str.size() != 4) return false;
	if (!(_str.at(0) >= 'a' && _str.at(0) <= 'z')) return false;
	if (!(_str.at(1) >= 'a' && _str.at(0) <= 'z')) return false;
	if (_str.at(2) != ' ') return false;
	if (!(_str.at(3) >= 'a' && _str.at(3) <= 'z')) return false;
	return true;
}

coord Command::getMovePos(const Board *board) const {
	if (!isMove()) return { -1,-1 };
	return board->getIndex(_str.substr(0, 2));
}

char Command::getMoveLetter() const {
	if (!isMove()) return '?';
	return toupper(_str.at(3));
}

bool Command::isExchange() const {
	return _str.size() == std::string("exchange").size() + 2
		&& _str.find("exchange") != std::string::npos;
}

char Command::getExchangeLetter() const {
	if (!isExchange()) return '?';
	return toupper(_str.at(_str.size() - 1));
}

bool Command::isCheckHands() const {
	return _str.find("hand") != std::string::npos;
}

bool Command::isCheckPool() const {
	return _str.find("pool") != std::string::npos;
}

bool Command::isHelp() const {
	return _str.find("help") != std::string::npos
		|| _str.find("tutorial") != std::string::npos;
}

bool Command::isHint() const {
	return _str.find("hint") != std::string::npos;
}