#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "containerUtil.h"
#include "consoleUtil.h"


struct coord{
    size_t vLine;
    size_t hColumn;

    bool operator==(const coord& l) const{
        return (l.vLine == vLine && l.hColumn == hColumn);
    }
};

struct codedWord{
    coord firstCoord;
    char orientation;
    std::string word;
};

class Board {
public:
	Board(size_t nLines = BOARD_MIN_DIM, size_t nColumns = BOARD_MIN_DIM);
    Board(const std::string &filename);
    void show() const;
	static coord getIndex(const std::string &position);
	static size_t getIndex(char letter);
    static std::string getPositionString(coord c);
	bool fileExport(const std::string &filename) const;
    bool highlight(int color, size_t vIndex, size_t hIndex);
    void highlightWordOnLine(int color, size_t vIndex, size_t hIndex);
    void highlightWordOnCol(int color, size_t vIndex, size_t hIndex);
    std::vector<std::vector<char>> getLetters() const;
    std::vector<char> getNonEmptyChars() const;
    std::vector<std::vector<bool>> getHighlights() const;
    coord getDimensions() const;
    bool addWord(codedWord word);
    std::vector<codedWord> getWords() const;
    bool removeWord(const std::string &wordToRemove);
    bool boardBounds(const codedWord &entry) const;
    static char getAlpha(size_t index, bool uppercase = true);


private:

    static std::string _alphabet;
    void defaultInit(size_t nLines = BOARD_MIN_DIM, size_t nColumns = BOARD_MIN_DIM);
    std::vector<std::vector<bool>> _highlights;
    std::vector<std::vector<int>> _highlightColors;
    std::vector<std::vector<char>> _letters;
    std::vector<codedWord> _words;
    size_t _vDimension;
    size_t _hDimension;

    bool checkIntersection(const codedWord &word) const;
    bool wordIsolation(const codedWord &word) const;
    codedWord* findWord (const std::string &word);
    bool wordExists(const std::string &word) const;
    std::vector<coord> getIntersectionsVector(const codedWord &word) const;
    void placeChar(coord inates, char character);

};

