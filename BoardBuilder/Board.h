#pragma once

#include <vector>
#include <string>

class Board {
public:
	Board();
	Board(std::vector<std::vector<char>> letters);
	void show() const;
	int getIndex(std::string position) const;
	void export(std::string filename) const;
	void import(std::string filename);
private:
	std::vector<std::vector<char>> _letters;
	unsigned _dimension;
};
