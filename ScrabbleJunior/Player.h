#pragma once

#include "Pool.h"
#include "Command.h"
#include "../common/Board.h"
#include <vector>
#include <string>

class Player {
public:
	Player(Pool *pool, const std::string &name, int color);
	int getScore() const;
	std::string getName() const;
	int getColor() const;
	void showHand(std::ostream& output, bool color) const;
	void addScore();
	void resetExchangeCount();
	void forcePass();
	void doNotPass();
	bool exchange(char letter, Pool *pool);
	bool takeRandom(int handPos, Pool *pool);
	bool mayMove(const Board *board) const;
	bool getMayPass() const;
	bool hasOnHand(char letter) const;
	int getHandPosition(char letter) const;
	int getExchangeCount() const;
	coord getPossibleMovePos(const Board* board) const;
	int getActualHandSize() const;
private:
	bool _mayPass;
	int _exchangeCount;
	int _color;
	std::string _name;
	int _score;
	std::vector<char> _hand;
};

