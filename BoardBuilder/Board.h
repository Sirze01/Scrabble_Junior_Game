#pragma once

#include "../ScrabbleJunior/Command.h"

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
    Board(); //needs default constructor if others are created
	Board(int nLines, int nCollumns);
    Board(std::string filename);
    void show() const;
	coord getIndex(std::string position) const;
	bool fileExport(std::string filename) const;
    bool move(Command command);
private:
    std::vector<std::vector<bool>> _highlights;
    std::vector<std::vector<char>> _letters;
    unsigned _vDimension;
    unsigned _hDimension;
    std::vector<std::string> _words; //needs reworking
};

