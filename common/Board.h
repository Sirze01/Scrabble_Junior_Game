#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../common/StringProcess.h"

struct coord{
    size_t vLine;
    size_t hCollumn;
};

struct codedWord{
    std::string firstCoord;
    std::string orientation;
    std::string word;

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
    void highlightFinishedWord(int color, int vIndex, int hIndex);
    std::vector<std::vector<char>> getLetters() const;
    std::vector<char> getNonEmptyChars() const;
    std::vector<std::vector<bool>> getHighlights() const;
    coord getDimensions() const;
    bool boardBounds(coord firstLetter, std::string orientation = "H", int wordLen = 0);
    bool goodIntersects(codedWord);
    void lettersManip(coord inates, char letter);
    void addWord(codedWord word);
    bool wordSpaces(codedWord word);

private:
    std::vector<std::vector<bool>> _highlights;
    std::vector<std::vector<int>> _highlightColors;
    std::vector<std::vector<char>> _letters;
    std::vector<codedWord> _words;
    size_t _vDimension;
    size_t _hDimension;
};

