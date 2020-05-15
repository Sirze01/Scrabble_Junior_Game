#pragma once

#include "Pool.h"
#include "Command.h"
#include "../common/Board.h"
#include <vector>
#include <string>

class Player {
public:
	Player(Pool &pool, const std::string &name, int color, bool isBot);
	int getScore() const;
	std::string getName() const;
	int getColor() const;
	void showHand(std::ostream& output, bool color) const;
	void addScore();
	void resetExchangeCount();
	void forcePass();
	void doNotPass();
	bool exchange(char letter, Pool &pool);
	bool takeRandom(int handPos, Pool &pool);
	bool mayMove(const Board &board) const;
	bool getHasPassed() const;
	bool hasOnHand(char letter) const;
	int getHandPosition(char letter) const;
	std::vector<char> getHand() const;
	int getExchangeCount() const;
	coord getPossibleMovePos(const Board &board) const;
	int getHandSize() const;
	bool isBot() const;
private:
	bool _hasPassed;
	int _exchangeCount;
	int _color;
	std::string _name;
	int _score;
	std::vector<char> _hand;
	bool _isBot;
};

