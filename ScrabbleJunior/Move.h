#pragma once
#include "Command.h"
#include "Player.h"
#include <ostream>
#include <vector>
#include <string>

class Move{
public:
	Move(Command command, Board board);
	bool isValid(Player player) const;
private:
	bool inBounds() const;
	bool letterMatch() const;
	bool startOnLine() const;
	bool startOnCol() const;
	bool continueOnLine() const;
	bool continueOnCol() const;
	bool finishOnLine() const;
	bool finishOnCol() const;
	coord _posToMove;
	char _letter;
	int _maxCol;
	int _maxLine;
	std::vector<std::vector<char>> _boardLetters;
	std::vector<std::vector<bool>> _boardHighlights;
};