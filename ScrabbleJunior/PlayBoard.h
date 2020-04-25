#pragma once
#include "Board.h"
#include "Player.h"

class PlayBoard : public Board {
public:
	bool inBounds(std::string position);
	bool isValid(std::string position, std::string letter);
	void write(std::string position, std::string letter);
	void showScores(std::vector<Player> players);
	void show();
};