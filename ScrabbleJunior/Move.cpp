#include "Move.h"
#include "Command.h"
#include "../common/Board.h"


Move::Move(Command command, Board board) {
	_posToMove = command.getMovePos(board);
	_letter = command.getMoveLetter();
	_maxCol = board.getDimensions().hCollumn - 1;
	_maxLine = board.getDimensions().vLine - 1;
	_boardLetters = board.getLetters();
	_boardHighlights = board.getHighlights();
}

Move::Move(coord pos, char letter, Board board) {
	_posToMove = pos;
	_letter = letter;
	_maxCol = board.getDimensions().hCollumn - 1;
	_maxLine = board.getDimensions().vLine - 1;
	_boardLetters = board.getLetters();
	_boardHighlights = board.getHighlights();
}

int Move::hasProblems(Player player) const {
	if (!letterMatch()) {
		return 1;
	}
	if (!player.hasOnHand(_letter)) {
		return 2;
	}
	if (_boardHighlights.at(_posToMove.vLine).at(_posToMove.hCollumn)) {
		return 3;
	}
	if (!inBounds()) {
		return 4;
	}

	if (singleCharWordOnLine() && singleCharWordOnCol()) return 0;
	else if (!singleCharWordOnLine() && continueOnLine()) return 0;
	else if (!singleCharWordOnCol() && continueOnCol()) return 0;

	return 5;
}

bool Move::execute(Player& player, Board& board, Pool &pool) {
	if (hasProblems(player)) return false;
	board.highlight(_posToMove.vLine, _posToMove.hCollumn);
	player.takeRandom(pool, player.getHandPosition(_letter));
	
	//add scores
	if (singleCharWordOnLine() && singleCharWordOnCol()) {
		player.addScore();
		return true;
	}
	if (!singleCharWordOnLine() && continueOnLine() && finishOnLine()) player.addScore();
	if (!singleCharWordOnCol() && continueOnCol() && finishOnCol()) player.addScore();

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