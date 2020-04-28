#pragma once

#include "Pool.h"
#include "Command.h"
#include "Board.h"

#include <vector>
#include <string>

class Player {
public:
	Player(Pool pool, std::string name);
	int getScore() const;
	std::string getName() const;
	void showHand() const;
	void addScore(int score);
	bool exchange(int pos1, int pos2, Pool &pool);
	bool exchange(int pos1, Pool& pool);
	bool takeRandom(Pool &pool, int handPosition);
	bool move(Command command, Board& board);
private:
	std::string _name;
	int _score;
	bool _turn;
	std::vector<char> _hand;
};

