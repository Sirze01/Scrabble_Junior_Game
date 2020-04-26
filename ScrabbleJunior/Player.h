#pragma once

#include "Pool.h"

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
	bool takeRandom(Pool &pool, int handPosition);
private:
	std::string _name;
	int _score;
	bool _turn;
	std::vector<char> _hand;
};

