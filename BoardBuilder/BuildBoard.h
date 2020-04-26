#pragma once
#include "Board.h"
class BuildBoard : public Board {
public:
	bool inBounds(std::string position, unsigned length, bool horizontal) const;
	bool overrides(std::string position, std::string word, bool horizontal) const;
	bool isValid(std::string position, std::string word, bool horizontal) const;
	void write(std::string position, std::string word, bool horizontal);
};

