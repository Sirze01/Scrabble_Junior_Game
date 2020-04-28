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

bool Move::isValid(Player player) const {
	if (!player.hasOnHand(_letter)) {
		std::cout << "Player hasn't got that letter in hand!\n";
		return false;
	}
	if (_boardHighlights.at(_posToMove.vLine).at(_posToMove.hCollumn)) {
		std::cout << "Position already filled with a letter!\n";
		return false;
	}
	if (!inBounds()) {
		std::cout << "Outside bounds!\n";
		return false;
	}
	if (!letterMatch()) {
		std::cout << "Letters do not match!\n";
		return false;
	}

	std::cout << "\nstart;finish;continue - LINE/COL\n";
	std::cout << startOnLine() << finishOnLine() << continueOnLine() << startOnCol() << finishOnCol() << continueOnCol();

	if ((startOnLine() && !finishOnLine()) || continueOnLine()) return true;
	if ((startOnCol() && !finishOnCol()) || continueOnCol()) return true;
	return false;
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

bool Move::startOnLine() const {
	if (_posToMove.hCollumn == 0) return true;
	return _boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn - 1) == ' ';
}

bool Move::startOnCol() const {
	if (_posToMove.vLine == 0) return true;
	return _boardLetters.at(_posToMove.vLine - 1).at(_posToMove.hCollumn) == ' ';
}

bool Move::continueOnLine() const {
	if (_posToMove.hCollumn == 0) return false;
	return _boardHighlights.at(_posToMove.vLine).at(_posToMove.hCollumn - 1)
		&& _boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn - 1) != ' ';
}

bool Move::continueOnCol() const {
	if (_posToMove.vLine == 0) return true;
	return _boardHighlights.at(_posToMove.vLine - 1).at(_posToMove.hCollumn)
		&& _boardLetters.at(_posToMove.vLine - 1).at(_posToMove.hCollumn) != ' ';
}

bool Move::finishOnLine() const {
	if (_posToMove.hCollumn == _maxCol) return true;
	return !_boardHighlights.at(_posToMove.vLine).at(_posToMove.hCollumn + 1)
		&& _boardLetters.at(_posToMove.vLine).at(_posToMove.hCollumn+1) == ' ';
}

bool Move::finishOnCol() const {
	if (_posToMove.vLine == _maxLine) return true;
	return !_boardHighlights.at(_posToMove.vLine+1).at(_posToMove.hCollumn)
		&& _boardLetters.at(_posToMove.vLine + 1).at(_posToMove.hCollumn) == ' ';
}