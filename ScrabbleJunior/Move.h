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
	bool execute(Player *player, Board *board, Pool *pool);
private:
	bool inBounds() const;
	bool letterMatch() const;
	bool startOnLine() const;
	bool startOnCol() const;
	bool continueOnLine() const;
	bool continueOnCol() const;
	bool finishOnLine() const;
	bool finishOnCol() const;
	bool singleCharWordOnLine() const;
	bool singleCharWordOnCol() const;
	coord _posToMove;
	char _letter;
	int _maxCol;
	int _maxLine;
	std::vector<std::vector<char>> _boardLetters;
	std::vector<std::vector<bool>> _boardHighlights;
};