#include "Move.h"
#include "Command.h"


Move::Move(const Command *command, const Board *board) {
	_posToMove = command->getMovePos(board);
	_letter = command->getMoveLetter();
	_maxCol = board->getDimensions().hCollumn - 1;
	_maxLine = board->getDimensions().vLine - 1;
	_boardLetters = board->getLetters();
	_boardHighlights = board->getHighlights();
}

Move::Move(const coord pos, char letter, const Board *board) {
	_posToMove = pos;
	_letter = letter;
	_maxCol = board->getDimensions().hCollumn - 1;
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
	if (_boardHighlights.at(_posToMove.vLine).at(_posToMove.hCollumn)) {
		return 4;
	}

	if (continueOnLine() && !singleCharWordOnLine()) return 0;
	if (continueOnCol() && !singleCharWordOnCol()) return 0;

	return 5;
}

bool Move::execute(Player *player, Board *board, Pool *pool, bool checkValidity) const {
	if (checkValidity && hasProblems(player)) return false;

	int color = player->getColor();
	board->highlight(color, _posToMove.vLine, _posToMove.hCollumn);

	player->takeRandom(player->getHandPosition(_letter),pool);
	
	//add scores and highlight ("territory dominance" feature)
	if ( (continueOnLine() && finishOnLine() && !singleCharWordOnLine())
		|| (continueOnCol() && finishOnCol() && !singleCharWordOnCol()) ) {
		player->addScore();
		board->highlightFinishedWord(player->getColor(), _posToMove.vLine, _posToMove.hCollumn);
	}

	player->resetExchangeCount();
	return true;
}

bool Move::inBounds() const {
	return _posToMove.hCollumn <= _maxCol
		&& _posToMove.vLine <= _maxLine
		&& _posToMove.hCollumn >= 0
		&& _posToMove.vLine >= 0;
}

bool Move::letterMatch() const {
    return _letter != ' '  && _letter == _boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn);
}

bool Move::singleCharWordOnLine() const {
	return startOnLine() && finishOnLine();
}

bool Move::singleCharWordOnCol() const {
	return startOnCol() && finishOnCol();
}

bool Move::startOnLine() const {
	return _posToMove.hCollumn == 0 || _boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn - 1) == ' ';
}

bool Move::startOnCol() const {
	return _posToMove.vLine == 0 || _boardLetters.at(_posToMove.vLine - 1).at(_posToMove.hCollumn) == ' ';
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
	for (int i = line + cof; i >= 0 && i <= _maxLine; i+=cof) {
		if (!_boardHighlights.at(i).at(_posToMove.hCollumn)) {

			if (_boardLetters.at(i).at(_posToMove.hCollumn) == ' ') {
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
	if (_posToMove.hCollumn == 0 && cof == -1) return true;
	if (_posToMove.hCollumn == _maxCol && cof == 1) return true;
	else if (cof != -1 && cof != 1) return false;

	bool completion = true; int col = _posToMove.hCollumn;
	for (int i = col + cof; i >= 0 && i <= _maxCol; i += cof) {
		if (!_boardHighlights.at(_posToMove.vLine).at(i)) {

			if (_boardLetters.at(_posToMove.vLine).at(i) == ' ') {
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