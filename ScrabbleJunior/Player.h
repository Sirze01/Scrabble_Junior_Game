#pragma once

#include "../common/Board.h"
#include "Pool.h"
#include "Command.h"
#include <vector>
#include <string>

class Player {
public:
	Player(Pool &pool, std::string name, int color, bool isBot);
	std::string getName() const;
	int getColor() const;
	int getScore() const;
	void showHand(std::ostream& output, bool color) const;
	void addScore();
	char exchange(char letter, Pool& pool);
	bool takeRandom(int handPos, Pool& pool);
	void resetExchangeCount();
	int getExchangeCount() const;
	bool getHasPassed() const;
	int getHandPosition(char letter) const;
	bool hasOnHand(char letter) const;
	bool mayMove(const Board& board) const;
	coord getPossibleMovePos(const Board& board) const;
	void forcePass();
	void doNotPass();
	int getHandSize() const;
	std::vector<char> getHand() const;
	bool isBot() const;
private:
	std::string _name;
	bool _hasPassed;
	int _score;
	int _color;
	int _exchangeCount;
	bool _isBot;
	std::vector<char> _hand;
};

