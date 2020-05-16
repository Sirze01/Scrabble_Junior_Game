#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../common/StringProcess.h"
#include "ConsoleSetup.h"
#include <algorithm>

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
    Board(std::string filename);
    void show() const;
	coord getIndex(const std::string &position) const;
    std::string getPositionString(coord c) const;
	bool fileExport(const std::string &filename) const;
    bool highlight(int color, int vIndex, int hIndex);
    void highlightFinishedWord(int color, int vIndex, int hIndex);
    std::vector<std::vector<char>> getLetters() const;
    std::vector<char> getNonEmptyChars() const;
    std::vector<std::vector<bool>> getHighlights() const;
    coord getDimensions() const;
    bool addWord(codedWord word);
    std::vector<codedWord> getWords() const;
    bool removeWord(std::string wordToRemove);
    bool boardBounds(const codedWord &entry);
    std::string getAlphabet() const;


private:

    static std::string _alphabet;
    void defaultInit(size_t nLines = BOARD_MIN_DIM, size_t nColumns = BOARD_MIN_DIM);
    std::vector<std::vector<bool>> _highlights;
    std::vector<std::vector<int>> _highlightColors;
    std::vector<std::vector<char>> _letters;
    std::vector<codedWord> _words;
    size_t _vDimension;
    size_t _hDimension;

    bool checkIntersection(codedWord) const;
    bool wordIsolation(codedWord word) const;
    codedWord* findWord (const std::string &word);
    bool wordExists(std::string word) const;
    std::vector<coord> getIntersectionsVector(codedWord word) const;
    void placeChar(coord inates, char character);

};

