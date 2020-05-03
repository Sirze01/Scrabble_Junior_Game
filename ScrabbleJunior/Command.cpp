#include "Command.h"
#include "../common/StringProcess.h"
#include "../common/Board.h"
#include <string>

Command::Command(std::string userInput) {
	_str = stripSpecialChars(userInput);
	_str = lowerCase(_str);
	_str = stripSpaces(_str);
	if (isExchange()) _exchangeToken = _str.at(_str.size() - 1);
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

int Command::getExchangeToken() const {
	if (!isExchange()) return -1;
	if (isdigit(_exchangeToken)) return _exchangeToken - '0';
	return _exchangeToken;
}

bool Command::isCheckHands() const {
	return _str.find("check hand") != std::string::npos;
}

bool Command::isCheckPool() const {
	return _str.find("check pool") != std::string::npos;
}

bool Command::isHelp() const {
	return _str.find("help") != std::string::npos;
}

bool Command::isHint() const {
	return _str.find("hint") != std::string::npos;
}