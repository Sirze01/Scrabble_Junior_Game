#include "Command.h"
#include <string>

Command::Command(std::string userInput) {
	_str = stripSpecialChars(userInput);
	_str = lowerCase(_str);
	_str = stripCommandBloat(_str);
	_str = stripSpaces(_str);
}

bool Command::isMove() const {
	return (_str.size() == 4) &&
	isalpha(_str.at(0)) &&
	isalpha(_str.at(1)) &&
	_str.at(2) == ' ' &&
	isalpha(_str.at(3)) ;
}

coord Command::getMovePos(const Board *board) const {
	if (!isMove()) return { -1,-1 };
	return board->getIndex(_str.substr(0, 2));
}

char Command::getMoveLetter() const {
	if (!isMove()) return '?';
	return toupper(_str.at(3));
}

std::string Command::getStr() const {
	return _str;
}

bool Command::isExchange(bool forceToken) const {
	bool hasExchange = _str.find("exchange") != std::string::npos;
	if (!forceToken) return hasExchange;

	return hasExchange
		&& _str.size() == std::string("exchange").size() + 2
		&& isalpha(_str.at(_str.size()-1));
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

bool Command::isCheckScores() const {
	return _str.find("score") != std::string::npos;
}

bool Command::isHelp() const {
	return _str.find("help") != std::string::npos
		|| _str.find("tutorial") != std::string::npos;
}

bool Command::isHint() const {
	return _str.find("hint") != std::string::npos;
}

bool Command::isPass() const {
	return _str.find("pass") != std::string::npos
		|| _str.find("skip") != std::string::npos;
}

bool Command::isClear() const {
	return _str.find("clear") != std::string::npos;
}

bool Command::hasNoConflicts() const {
	int count = 0;
	if (isExchange(false)) count++;
	if (isCheckHands()) count++;
	if (isCheckPool()) count++;
	if (isCheckScores()) count++;
	if (isHelp()) count++;
	if (isHint()) count++;
	if (isPass()) count++;
	if (isClear()) count++;
	return count < 2;
}