#pragma once
#include "Board.h"
class BuildBoard : public Board {
public:
	bool inBounds(std::string position, unsigned length, bool horizontal);
	bool overrides(std::string position, std::string word, bool horizontal);
	bool isValid(std::string position, std::string word, bool horizontal);
	void write(std::string position, std::string word, bool horizontal);
};

