#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../common/StringProcess.h"

struct coord{
    size_t vLine;
    size_t hCollumn;

    bool operator==(const coord& l) const
    {
        return (l.vLine == vLine && l.hCollumn == hCollumn);
    }
};

struct codedWord{
    coord firstCoord;
    char orientation;
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
    std::string indexToLetter(coord coordinates) const;
    bool boardBounds(const codedWord &entry);
    void placeChar(coord inates, char character);
    std::string getAlphabet() const;
    std::vector<coord> checkIntersections(codedWord word);

private:
    std::string _alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::vector<std::vector<bool>> _highlights;
    std::vector<std::vector<int>> _highlightColors;
    std::vector<std::vector<char>> _letters;
    std::vector<codedWord> _words;
    size_t _vDimension;
    size_t _hDimension;
};

