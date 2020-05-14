#pragma once

#include "Command.h"
#include "Player.h"
#include <ostream>
#include <vector>
#include <string>

class Move {
public:
	Move(const Command *command, const Board *board);
	Move(coord pos, char letter, const Board *board);
	int hasProblems(const Player *player) const;
	bool execute(Player *player, Board *board, Pool *pool, bool checkValidity = false) const;
private:
	bool inBounds() const;
	bool letterMatch() const;
	bool startOnLine() const;
	bool startOnCol() const;
	bool continueOnLine() const;
	bool continueOnCol() const;
	bool finishOnLine() const;
	bool finishOnCol() const;
	bool wordCompletionOnCol(int cof) const;
	bool wordCompletionOnLine(int cof) const;
	bool singleCharWordOnLine() const;
	bool singleCharWordOnCol() const;
	coord _posToMove;
	char _letter;
	size_t _maxCol;
	size_t _maxLine;
	std::vector<std::vector<char>> _boardLetters;
	std::vector<std::vector<bool>> _boardHighlights;
};