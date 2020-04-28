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
    Board(); //needs default constructor if others are created
	Board(int nLines, int nCollumns);
    Board(std::string filename);
    void show() const;
	coord getIndex(std::string position) const;
	bool fileExport(std::string filename) const;
    bool highlight(int vIndex, int hIndex);
    std::vector<std::vector<char>> getLetters() const;
    std::vector<std::vector<bool>> getHighlights() const;
    coord getDimensions() const;
private:
    std::vector<std::vector<bool>> _highlights;
    std::vector<std::vector<char>> _letters;
    unsigned _vDimension;
    unsigned _hDimension;
    std::vector<std::string> _words; //needs reworking
};

