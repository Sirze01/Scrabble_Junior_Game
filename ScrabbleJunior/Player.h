#pragma once

#include "Pool.h"
#include "Command.h"
#include "../common/Board.h"
#include <vector>
#include <string>

class Player {
public:
	Player(Pool *pool, std::string name);
	int getScore() const;
	std::string getName() const;
	void showHand() const;
	void showScore() const;
	void addScore();
	bool exchange(char letter, Pool *pool);
	bool takeRandom(int handPos, Pool *pool);
	bool mayMove(const Board *board, const Pool *pool) const;
	bool hasOnHand(char letter) const;
	int getHandPosition(char letter) const;
	coord getPossiblePos(const Board* board, const Pool* pool) const;
private:
	int _colorCode;
	std::string _colorName;
	std::string _name;
	int _score;
	std::vector<char> _hand;
};

