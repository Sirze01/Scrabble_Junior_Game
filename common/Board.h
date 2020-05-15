#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../common/StringProcess.h"
#include "ConsoleSetup.h"

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
	bool fileExport(std::string filename) const;
    bool highlight(int color, int vIndex, int hIndex);
    void highlightWordOnLine(int color, int vIndex, int hIndex);
    void highlightWordOnCol(int color, int vIndex, int hIndex);
    std::vector<std::vector<char>> getLetters() const;
    std::vector<char> getNonEmptyChars() const;
    std::vector<std::vector<bool>> getHighlights() const;
    coord getDimensions() const;
    void addWord(codedWord word);
    bool wordSpaces(codedWord word);
    std::vector<codedWord> getWords() const;
    //Testing
    void removeWord(codedWord word);
    codedWord* findWord (std::string word);
    codedWord* findWord (coord inates);
    bool wordExists(std::string word) const;
    bool wordExists(coord inates) const;
    void removeLetter(coord inates, char letter);
    bool checkIntersection(codedWord);
    bool boardBounds(const codedWord &entry);
    void placeChar(coord inates, char character);
    std::string getAlphabet() const;
    std::vector<coord> checkIntersections(codedWord word);

private:

    std::string _alphabet = "abcdefghijklmnopqrstuvwxyz";
    void defaultInit(size_t nLines = BOARD_MIN_DIM, size_t nColumns = BOARD_MIN_DIM);
    std::vector<std::vector<bool>> _highlights;
    std::vector<std::vector<int>> _highlightColors;
    std::vector<std::vector<char>> _letters;
    std::vector<codedWord> _words;
    size_t _vDimension;
    size_t _hDimension;
};

