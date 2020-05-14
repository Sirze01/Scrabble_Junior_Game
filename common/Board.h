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
    char orientation = '\0';
    std::string word;
    std::vector<coord> intersections;

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
    bool boardBounds(coord firstLetter, char orientation = 'H', int wordLen = 0);
    void placeChar(coord inates, char character);
    void addWord(codedWord word);
    bool wordSpaces(codedWord word);
    std::string getAlphabet() const;
    std::vector<codedWord> getWords() const;
    //Testing
    void removeWord(codedWord word);
    codedWord findWord (std::string word) const;
    void removeLetter(coord inates, char letter);
    bool checkIntersection(codedWord);

private:
    std::string _alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::vector<std::vector<bool>> _highlights;
    std::vector<std::vector<int>> _highlightColors;
    std::vector<std::vector<char>> _letters;
    std::vector<codedWord> _words;
    int _vDimension;
    int _hDimension;
};

