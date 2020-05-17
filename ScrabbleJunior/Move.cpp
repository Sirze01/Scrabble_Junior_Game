#include "Move.h"
#include <utility>

/**
 * A move must contain board coordinates and a letter to play.
 * @param command - to extract position and letter.
 * @param board - to then modify highlights and colors while executing.
 */
Move::Move(const Command &command, const Board &board):
	_posToMove{ command.getMovePos() }, _letter{ command.getMoveLetter() },
	_maxCol{ board.getDimensions().hColumn - 1 }, _maxLine{board.getDimensions().vLine - 1 },
	_boardLetters{ board.getLetters() }, _boardHighlights{ board.getHighlights() }
{
}

/**
 * A move must contain board coordinates and a letter to play.
 * @param pos - board position to play in.
 * @param letter - letter to play.
 * @param board - to then modify highlights and colors while executing.
 */
Move::Move(const coord &pos, char letter, const Board &board) :
	_posToMove{ pos }, _letter{ letter },
	_maxCol{ board.getDimensions().hColumn - 1 }, _maxLine{ board.getDimensions().vLine - 1 },
	_boardLetters{ board.getLetters() }, _boardHighlights{ board.getHighlights() }
{
}

/**
 * Check whether a move has problems and if so, which.
 * @param player - to check hand.
 * @return
 * 0 - move is legal;
 * 1 - move is outside board bounds;
 * 2 - given letter does not match the board at given position;
 * 3 - player does not have given letter in hand;
 * 4 - position has already been filled with a tile (highlight is true);
 * 5 - move does not start or continue a word.
 */
int Move::hasProblems(const Player &player) const {
	if (!inBounds()) {
		return 1;
	}
	if (!letterMatch()) {
		return 2;
	}
	if (!player.hasOnHand(_letter)) {
		return 3;
	}
	if (_boardHighlights.at(_posToMove.vLine).at(_posToMove.hColumn)) {
		return 4;
	}

	if (continueOnLine() && !singleCharWordOnLine()) return 0;
	if (continueOnCol() && !singleCharWordOnCol()) return 0;

	return 5;
}

/**
 * Highlight board, update scores if completed word, take from the pool.
 * @param player - to change hand.
 * @param board - to change highlights.
 * @param pool - to take letter.
 */
void Move::execute(Player &player, Board &board, Pool &pool) const {
	int color = player.getColor();
	board.highlight(color, _posToMove.vLine, _posToMove.hColumn);

	player.takeRandom(player.getHandPosition(_letter),pool);

	auto scoreAction = [&](bool line) { //add scores and highlight ("territory dominance" feature)
		player.addScore();
		if (line) board.highlightWordOnLine(player.getColor(), _posToMove.vLine, _posToMove.hColumn);
		else board.highlightWordOnCol(player.getColor(), _posToMove.vLine, _posToMove.hColumn);
	};
	
	if (continueOnLine() && finishOnLine() && !singleCharWordOnLine()) scoreAction(true);
	if (continueOnCol() && finishOnCol() && !singleCharWordOnCol()) scoreAction(false);
}

/** Check if a move is outside board bounds. */
bool Move::inBounds() const {
	return _posToMove.hColumn <= _maxCol
		&& _posToMove.vLine <= _maxLine;
}

/** Check if given letter does not match */
bool Move::letterMatch() const {
    return _letter != SPACE  && _letter == _boardLetters.at(_posToMove.vLine).at(_posToMove.hColumn);
}

/** Check if there's no letter immediatly before and after on the line. */
bool Move::singleCharWordOnLine() const {
	return startOnLine() && finishOnLine();
}

/* Check if there's no letter immediatly before and after on the line. */
bool Move::singleCharWordOnCol() const {
	return startOnCol() && finishOnCol();
}

/** Check if there's no letter immediatly before on the line.*/
bool Move::startOnLine() const {
	return _posToMove.hColumn == 0 || _boardLetters.at(_posToMove.vLine).at(_posToMove.hColumn - 1) == SPACE;
}

/** Check if there's no letter immediatly before on the column.*/
bool Move::startOnCol() const {
	return _posToMove.vLine == 0 || _boardLetters.at(_posToMove.vLine - 1).at(_posToMove.hColumn) == SPACE;
}

/** Check on line if all letters before are highlighted. Breaks if space is found. */
bool Move::continueOnLine() const {
	return wordCompletionOnLine(-1);
}

/** Check on column if all letters before are highlighted. Breaks if space is found. */
bool Move::continueOnCol() const {
	return wordCompletionOnCol(-1);
}

/** Check on line if all letters after are highlighted. Breaks if space is found. */
bool Move::finishOnLine() const {
	return wordCompletionOnLine(1);
}

/** Check on line if all letters after are highlighted. Breaks if space is found. */
bool Move::finishOnCol() const {
	return wordCompletionOnCol(1);
}

/**
 * Iterate the line to make sure letters are highlited until space is found.
 * @param cof - direction of iteration
 * @return true - word is completed
 * @return false - word is not completed
 */
bool Move::wordCompletionOnLine(int cof) const {
	if ((_posToMove.hColumn == 0 && cof == -1) ||
		(_posToMove.hColumn == _maxLine && cof == 1)) return true;

	for (size_t i = _posToMove.hColumn + cof; i <= _maxCol; i += cof) {
		if (!_boardHighlights.at(_posToMove.vLine).at(i)) {
			return _boardLetters.at(_posToMove.vLine).at(i) == SPACE;
		}
	}
	return true;
}

/**
 * Iterate the column to make sure letters are highlited until space is found.
 * @param cof - direction of iteration
 * @return true - word is completed
 * @return false - word is not completed
 */
bool Move::wordCompletionOnCol(int cof) const {
	if ((_posToMove.vLine == 0 && cof == -1) ||
		(_posToMove.vLine == _maxLine && cof == 1)) return true;

	for (size_t i = _posToMove.vLine + cof; i <= _maxLine; i+=cof) {
		if (!_boardHighlights.at(i).at(_posToMove.hColumn)) {
			return _boardLetters.at(i).at(_posToMove.hColumn) == SPACE;
		}
	}
	return true;
}