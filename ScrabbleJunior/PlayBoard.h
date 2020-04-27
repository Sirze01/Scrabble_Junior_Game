#pragma once
#include "../BoardBuilder/Board.h"
#include "Player.h"
#include "Command.h"

class PlayBoard : public Board {
public:
	PlayBoard(std::string filename);
	bool move(Command command);
	//void highlight() const; //to be used with show();
	//void showScores(std::vector<Player> players) const;
private:
	std::vector<std::vector<bool>> _highlights;
	//bool inBounds(std::string position) const;
	//bool isValid(std::string position, std::string letter) const;
	//void write(std::string position, std::string letter);
};