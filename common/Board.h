#pragma once

#include "consoleUtil.h"
#include "containerUtil.h"
#include <fstream>
#include <algorithm>


struct Coord{
    size_t line;
    size_t col;

    bool operator==(const Coord& l) const{
        return (l.line == line && l.col == col);
    }
};

struct Word{
    Coord firstLetterPos;
    char orientation;
    std::string str;
};

class Board {
public:
	Board(size_t nLines = BOARD_MIN_DIM, size_t nColumns = BOARD_MIN_DIM);
    Board(const std::string &filename);
    void show() const;
	static Coord getIndex(const std::string &position);
	static size_t getIndex(char letter);
    static std::string getPositionString(Coord c);
	bool fileExport(const std::string &filename) const;
    bool highlight(int color, size_t vIndex, size_t hIndex);
    void highlightWordOnLine(int color, size_t vIndex, size_t hIndex);
    void highlightWordOnCol(int color, size_t vIndex, size_t hIndex);
    std::vector<std::vector<char>> getLetters() const;
    std::vector<char> getNonEmptyChars() const;
    std::vector<std::vector<bool>> getHighlights() const;
    Coord getDimensions() const;
    bool addWord(Word word, int &statusCode);
    std::vector<Word> getWords() const;
    bool removeWord(const std::string &wordToRemove, int &statusCode);
    bool boardBounds(const Word &entry) const;
    static char getAlpha(size_t index, bool uppercase = true);

private:
    void defaultInit(size_t nLines = BOARD_MIN_DIM, size_t nColumns = BOARD_MIN_DIM);
    std::vector<std::vector<bool>> _highlights;
    std::vector<std::vector<int>> _highlightColors;
    std::vector<std::vector<char>> _letters;
    std::vector<Word> _words;
    size_t _nLines;
    size_t _nCols;

    bool checkIntersection(const Word &word) const;
    bool wordIsolation(const Word &word) const;
    Word* findWord (const std::string &word);
    bool wordExists(const std::string &word) const;
    std::vector<Coord> getIntersectionsVector(const Word &word) const;
    void placeChar(Coord inates, char character);

};

