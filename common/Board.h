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
    Board();
	Board(int nLines, int nCollumns);
    Board(std::string filename);
    void show() const;
	coord getIndex(std::string position) const;
	bool fileExport(std::string filename) const;
    bool highlight(int color, int vIndex, int hIndex);
    bool highlightFinishedWord(int color, int vIndex, int hIndex);
    std::vector<std::vector<char>> getLetters() const;
    std::vector<std::vector<bool>> getHighlights() const;
    coord getDimensions() const;
private:
    std::vector<std::vector<bool>> _highlights;
    std::vector<std::vector<int>> _highlightColors;
    std::vector<std::vector<char>> _letters;
    int _vDimension;
    int _hDimension;
    std::vector<std::string> _words;
};

