#pragma once
#include <string>
#include <vector>
#include "../common/Board.h"

class Command {
public:
	Command(std::string userInput);
	int getCommand() const; //list of commands outside move
    coord getMovePos(Board board) const;
	char getMoveLetter() const;
	bool isMove() const;
private:
	std::string _str;
	int _commandType;
};