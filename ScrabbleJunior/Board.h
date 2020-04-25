#pragma once

#include <vector>
#include <string>

class Board {
public:
	Board();
	Board(std::vector<std::vector<char>> letters);
	void show();
	int getIndex(std::string position);
	void export(std::string filename);
	void import(std::string filename);
private:
	std::vector<std::vector<char>> _letters;
	unsigned _dimension;
};
