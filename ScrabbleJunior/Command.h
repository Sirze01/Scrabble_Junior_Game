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
	bool isMove() const;
	int getExchangeToken() const;
	bool isExchange() const;
	bool isCheckPool() const;
	bool isCheckHands() const;
	bool isHelp() const;
	bool isHint() const;
private:
	std::string _str;
	int _commandType;
	int _exchangeToken; //letter or handPos
};