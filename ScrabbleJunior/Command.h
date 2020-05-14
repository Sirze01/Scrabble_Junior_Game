#pragma once

#include <string>
#include <vector>
#include "../common/ConsoleSetup.h"
#include "Player.h"
#include "../common/Board.h"

class Command {
public:
	Command(std::string &userInput);
    coord getMovePos(const Board *board) const;
	char getMoveLetter() const;
	char getExchangeLetter() const;
	std::string getStr() const;
	bool isMove() const;
	bool isExchange(bool forceToken = true) const;
	bool isCheckPool() const;
	bool isCheckHands() const;
	bool isCheckScores() const;
	bool isHint() const;
	bool isHelp() const;
	bool isPass() const;
	bool isClear() const;
	bool hasNoConflicts() const;
private:
	std::string _str;
};