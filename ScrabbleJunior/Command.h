#pragma once
#include <string>
#include <vector>
#include "Player.h"
#include "../common/Board.h"

class Command {
public:
	Command(std::string userInput);
    coord getMovePos(const Board *board) const;
	char getMoveLetter() const;
	char getExchangeLetter() const;
	bool isMove() const;
	bool isExchange() const;
	bool isCheckPool() const;
	bool isCheckHands() const;
	bool isCheckScores() const;
	bool isHint() const;
	bool isHelp() const;
	bool hasNoConflicts() const;
private:
	std::string _str;
};