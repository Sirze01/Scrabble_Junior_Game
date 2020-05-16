#pragma once

#include "../common/Board.h"
#include <string>
#include <vector>

class Command {
public:
	Command(const std::string &userInput);
	bool isMove() const;
    coord getMovePos(const Board &board) const;
	char getMoveLetter() const;
	bool isExchange(bool forceToken = true) const;
	char getExchangeLetter() const;
	bool isCheckHands() const;
	bool isCheckPool() const;
	bool isCheckScores() const;
	bool isHelp() const;
	bool isHint() const;
	bool isPass() const;
	bool isClear() const;
	bool hasNoConflicts() const;
private:
	std::string _str;
};