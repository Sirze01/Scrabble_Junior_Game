#pragma once

#include "Command.h"
#include "Player.h" //includes "Board.h"

class Move {
public:
	Move(const Command &command, const Board &board);
	Move(const Coord &pos, char letter, const Board &board);
	int hasProblems(const Player &player) const;
	void execute(Player &player, Board &board, Pool &pool) const;
private:
	bool inBounds() const;
	bool letterMatch() const;
	bool singleCharWordOnLine() const;
	bool singleCharWordOnCol() const;
	bool startOnLine() const;
	bool startOnCol() const;
	bool continueOnLine() const;
	bool continueOnCol() const;
	bool finishOnLine() const;
	bool finishOnCol() const;
	bool wordCompletionOnLine(int cof) const;
	bool wordCompletionOnCol(int cof) const;

	Coord _posToMove;
	char _letter;
	size_t _maxCol;
	size_t _maxLine;
	std::vector<std::vector<char>> _boardLetters;
	std::vector<std::vector<bool>> _boardHighlights;
};