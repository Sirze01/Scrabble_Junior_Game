#include "Move.h"
#include "Command.h"


Move::Move(const Command *command, const Board *board) {
	_posToMove = command->getMovePos(board);
	_letter = command->getMoveLetter();
	_maxCol = board->getDimensions().hColumn - 1;
	_maxLine = board->getDimensions().vLine - 1;
	_boardLetters = board->getLetters();
	_boardHighlights = board->getHighlights();
}

Move::Move(const coord pos, char letter, const Board *board) {
	_posToMove = pos;
	_letter = letter;
	_maxCol = board->getDimensions().hColumn - 1;
	_maxLine = board->getDimensions().vLine - 1;
	_boardLetters = board->getLetters();
	_boardHighlights = board->getHighlights();
}

int Move::hasProblems(const Player *player) const {
	if (!inBounds()) {
		return 1;
	}
	if (!letterMatch()) {
		return 2;
	}
	if (!player->hasOnHand(_letter)) {
		return 3;
	}
	if (_boardHighlights.at(_posToMove.vLine).at(_posToMove.hColumn)) {
		return 4;
	}

	if (continueOnLine() && !singleCharWordOnLine()) return 0;
	if (continueOnCol() && !singleCharWordOnCol()) return 0;

	return 5;
}

bool Move::execute(Player *player, Board *board, Pool *pool, bool checkValidity) const {
	if (checkValidity && hasProblems(player)) return false;

	int color = player->getColor();
	board->highlight(color, _posToMove.vLine, _posToMove.hColumn);

	player->takeRandom(player->getHandPosition(_letter),pool);
	
	//add scores and highlight ("territory dominance" feature)
	if ( (continueOnLine() && finishOnLine() && !singleCharWordOnLine())
		|| (continueOnCol() && finishOnCol() && !singleCharWordOnCol()) ) {
		player->addScore();
		board->highlightFinishedWord(player->getColor(), _posToMove.vLine, _posToMove.hColumn);
	}

	player->resetExchangeCount();
	return true;
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
	if (_posToMove.vLine == 0 && cof == -1) return true;
	if (_posToMove.vLine == _maxLine && cof == 1) return true;
	else if (cof != -1 && cof != 1) return false;

	bool completion = true; int line = _posToMove.vLine;
	for (int i = line + cof; i >= 0 && i <= static_cast<int>(_maxLine); i+=cof) {
		if (!_boardHighlights.at(i).at(_posToMove.hColumn)) {

			if (_boardLetters.at(i).at(_posToMove.hColumn) == SPACE) {
				break;
			}
			else {
				completion = false;
				break;
			}
		}
	}
	return completion;
}

bool Move::wordCompletionOnLine(int cof) const {
	if (_posToMove.hColumn == 0 && cof == -1) return true;
	if (_posToMove.hColumn == _maxCol && cof == 1) return true;
	else if (cof != -1 && cof != 1) return false;

	bool completion = true; int col = _posToMove.hColumn;
	for (int i = col + cof; i >= 0 && i <= static_cast<int>(_maxCol); i += cof) {
		if (!_boardHighlights.at(_posToMove.vLine).at(i)) {

			if (_boardLetters.at(_posToMove.vLine).at(i) == SPACE) {
				break;
			}
			else {
				completion = false;
				break;
			}
		}
	}
	return completion;
}