#include "Move.h"
#include "Command.h"
#include "../common/Board.h"


Move::Move(const Command *command, const Board *board) {
	_posToMove = command->getMovePos(board);
	_letter = command->getMoveLetter();
	_maxCol = board->getDimensions().hCollumn - 1;
	_maxLine = board->getDimensions().vLine - 1;
	_boardLetters = board->getLetters();
	_boardHighlights = board->getHighlights();
}

Move::Move(coord pos, char letter, const Board *board) {
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

	if (!singleCharWordOnLine() && continueOnLine()) return 0;
	if (!singleCharWordOnCol() && continueOnCol()) return 0;

	return 5;
}

bool Move::execute(Player *player, Board *board, Pool *pool) {
	if (hasProblems(player)) return false;

	int color = player->getColor();
	board->highlight(color, _posToMove.vLine, _posToMove.hCollumn);

	player->takeRandom(player->getHandPosition(_letter),pool);
	
	//add scores and highlight ("territory dominance" feature)
	if ( (!singleCharWordOnLine() && continueOnLine() && finishOnLine())
		|| (!singleCharWordOnCol() && continueOnCol() && finishOnCol()) ) {
		player->addScore();
		board->highlightFinishedWord(player->getColor(), _posToMove.vLine, _posToMove.hCollumn);
	}

	player->resetExchangeCount();
	return true;
}

bool Move::inBounds() const {
	if (_posToMove.hCollumn > _maxCol) return false;
	if (_posToMove.vLine > _maxLine) return false;
	if (_posToMove.hCollumn < 0) return false;
	if (_posToMove.vLine < 0) return false;
	return true;
}

bool Move::letterMatch() const {
	if (_letter == ' ') return false;
	return _letter == _boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn);
}

bool Move::singleCharWordOnLine() const {
	return startOnLine() && finishOnLine();
}

bool Move::singleCharWordOnCol() const {
	return startOnCol() && finishOnCol();
}

bool Move::startOnLine() const {
	if (_posToMove.hCollumn == 0) return true;
	return _boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn - 1) == ' ';
}

bool Move::startOnCol() const {
	if (_posToMove.vLine == 0) return true;
	return _boardLetters.at(_posToMove.vLine - 1).at(_posToMove.hCollumn) == ' ';
}


bool Move::continueOnLine() const {
	if (_posToMove.hCollumn == 0) return true;
	bool continueWord = true;
	for (int i = 1; _posToMove.hCollumn - i >= 0; ++i) {
		if (!_boardHighlights.at(_posToMove.vLine).at(_posToMove.hCollumn - i)) {
			if (_boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn - i) == ' ') break;
			else if (_boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn - i) != ' ') {
				continueWord = false;
				break;
			}
		}
	}
	return continueWord;
}

bool Move::continueOnCol() const {
	if (_posToMove.vLine == 0) return true;
	bool continueWord = true;
	for (int i = 1; _posToMove.vLine - i >= 0; ++i) {
		if (!_boardHighlights.at(_posToMove.vLine - i).at(_posToMove.hCollumn)) {
			if (_boardLetters.at(_posToMove.vLine - i).at(_posToMove.hCollumn) == ' ') break;
			else if (_boardLetters.at(_posToMove.vLine - i).at(_posToMove.hCollumn) != ' ') {
				continueWord = false;
				break;
			}
		}
	}
	return continueWord;
}

bool Move::finishOnLine() const {
	if (_posToMove.hCollumn == _maxCol) return true;
	bool finishWord = true;
	for (int i = 1; _posToMove.hCollumn + i <= _maxCol; ++i) {
		if (!_boardHighlights.at(_posToMove.vLine).at(_posToMove.hCollumn + i)) {
			if (_boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn + i) == ' ') break;
			else if (_boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn + i) != ' ') {
				finishWord = false;
				break;
			}
		}
	}
	return finishWord;
}

bool Move::finishOnCol() const {
	if (_posToMove.vLine == _maxLine) return true;
	bool finishWord = true;
	for (int i = 1; _posToMove.vLine + i <= _maxLine; ++i) {
		if (!_boardHighlights.at(_posToMove.vLine+i).at(_posToMove.hCollumn)) {
			if (_boardLetters.at(_posToMove.vLine+i).at(_posToMove.hCollumn) == ' ') break;
			else if (_boardLetters.at(_posToMove.vLine + i).at(_posToMove.hCollumn) != ' ') {
				finishWord = false;
				break;
			}
		}
	}
	return finishWord;
}