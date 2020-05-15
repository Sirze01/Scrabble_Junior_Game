#include "Move.h"
#include "Command.h"

Move::Move(const Command &command, const Board &board):
	_maxCol{ board.getDimensions().hColumn - 1 }, _maxLine{board.getDimensions().vLine - 1 },
	_boardLetters{ board.getLetters() }, _boardHighlights{ board.getHighlights() },
	_posToMove{ command.getMovePos(board) }, _letter{ command.getMoveLetter() }
{
}

Move::Move(const coord &pos, char letter, const Board &board) :
	_maxCol{ board.getDimensions().hColumn - 1 }, _maxLine{ board.getDimensions().vLine - 1 },
	_boardLetters{ board.getLetters() }, _boardHighlights{ board.getHighlights() },
	_posToMove{ pos }, _letter{ letter }
{
}

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

void Move::execute(Player &player, Board &board, Pool &pool) const {
	int color = player.getColor();
	board.highlight(color, _posToMove.vLine, _posToMove.hColumn);

	player.takeRandom(player.getHandPosition(_letter),pool);

	auto scoreAction = [&](const std::string &direction) { //add scores and highlight ("territory dominance" feature)
		player.addScore();
		if (direction == "line") board.highlightWordOnLine(player.getColor(), _posToMove.vLine, _posToMove.hColumn);
		else board.highlightWordOnCol(player.getColor(), _posToMove.vLine, _posToMove.hColumn);
	};
	
	if (continueOnLine() && finishOnLine() && !singleCharWordOnLine()) scoreAction("line");
	if (continueOnCol() && finishOnCol() && !singleCharWordOnCol()) scoreAction("column");
}

bool Move::inBounds() const {
	return _posToMove.hColumn <= _maxCol
		&& _posToMove.vLine <= _maxLine;
}

bool Move::letterMatch() const {
    return _letter != SPACE  && _letter == _boardLetters.at(_posToMove.vLine).at(_posToMove.hColumn);
}

bool Move::singleCharWordOnLine() const {
	return startOnLine() && finishOnLine();
}

bool Move::singleCharWordOnCol() const {
	return startOnCol() && finishOnCol();
}

bool Move::startOnLine() const {
	return _posToMove.hColumn == 0 || _boardLetters.at(_posToMove.vLine).at(_posToMove.hColumn - 1) == SPACE;
}

bool Move::startOnCol() const {
	return _posToMove.vLine == 0 || _boardLetters.at(_posToMove.vLine - 1).at(_posToMove.hColumn) == SPACE;
}

bool Move::continueOnLine() const {
	return wordCompletionOnLine(-1);
}

bool Move::continueOnCol() const {
	return wordCompletionOnCol(-1);
}

bool Move::finishOnLine() const {
	return wordCompletionOnLine(1);
}

bool Move::finishOnCol() const {
	return wordCompletionOnCol(1);
}

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