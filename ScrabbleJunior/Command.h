#pragma once
#include <string>
#include <vector>

class Command {
public:
	Command(std::string userInput);
	int getCommand() const; //list of commands outside move
	std::vector<std::string> getMove() const;
	bool isMove() const;
private:
	void saveMove();
	std::string _str;
	std::vector<std::string> _move;
	int _commandType;
};

