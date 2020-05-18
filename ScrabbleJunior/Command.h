#pragma once

#include "../common/Board.h"

class Command {
public:
	explicit Command(const std::string &userInput);
	bool isMove() const;
    Coord getMovePos() const;
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
	std::string getCommandStr() const;

private:
	std::string _str;
};