#pragma once

#include <vector>
#include <string>

struct coord{
    int horizontal;
    int vertical;
};

class Board {
public:
	Board();
    void show() const;
	coord getIndex(std::string position) const;
	void fileExport(std::string filename) const;
	void fileImport(std::string filename) const;
private:
    Board(std::vector<std::vector<char>> letters);
    std::vector<std::vector<char>> _letters;
	unsigned _dimension;
};

