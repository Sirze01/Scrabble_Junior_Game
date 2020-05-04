#pragma once

#include "Pool.h"
#include "Command.h"
#include "../common/Board.h"
#include <vector>
#include <string>

class Player {
public:
	Player(Pool *pool, std::string name, int color);
	int getScore() const;
	std::string getName() const;
	int getColor() const;
	void showHand() const;
	void showScore() const;
	void addScore();
	void resetExchangeCount();
	bool exchange(char letter, Pool *pool);
	bool takeRandom(int handPos, Pool *pool);
	bool mayMove(const Board *board, const Pool *pool) const;
	bool mustPass() const;
	bool hasOnHand(char letter) const;
	int getHandPosition(char letter) const;
	coord getPossiblePos(const Board* board, const Pool* pool) const;
private:
	int _exchangeCount;
	int _color;
	std::string _name;
	int _score;
	std::vector<char> _hand;
};

