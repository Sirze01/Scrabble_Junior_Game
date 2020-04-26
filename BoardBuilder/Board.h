#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct coord{
    int vLine;
    int hCollumn;
};


class Board {
public:

	Board(int nLines, int nCollumns);
    Board(std::string filename);
    void show() const;
	coord getIndex(std::string position) const;
	void fileExport(std::string filename) const;
    //Board();
    //Board(std::vector<std::vector<char>> letters);

private:

    std::vector<std::vector<char>> _letters;
    unsigned _vDimension;
    unsigned _hDimension;
    std::vector<std::string> _words;
};

