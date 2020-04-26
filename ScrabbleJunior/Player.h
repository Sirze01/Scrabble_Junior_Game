#pragma once

#include "Pool.h"

#include <vector>
#include <string>


class Player {
public:
	Player(Pool pool);
	void addScore(int score);
	int getScore();
	void updateHand(Pool pool);
	std::string getName();
private:
	std::string _name;
	int _score;
	bool _turn;
	char _hand[7];
};

