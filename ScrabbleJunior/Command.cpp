#include "Command.h"

/**
 * A command contains a string which defines its action on the game.
 * The string is treated and some command keywords are removed to allow more flexibility.
 * The actions of the command are handled by Game.
 * @param userInput - command string.
 */
Command::Command(const std::string &userInput) : _str{userInput} {
	Util::stripSpecialChars(_str);
	Util::lowerCase(_str);
	Util::stripCommandBloat(_str);
	Util::stripSpaces(_str);
}

/**
 * Check if the command is a move (not necessarily valid), ie. of the form '<Yx> <letter>'
 * @return true 
 * @return false 
 */
bool Command::isMove() const {
	return (_str.size() == 4) &&
		std::isalpha(_str.at(0)) &&
		std::isalpha(_str.at(1)) &&
		_str.at(2) == SPACE &&
		std::isalpha(_str.at(3));
}

/**
 * Assuming the command represents a move, gets its coordinates on the board.
 * @param board - to extract the index
 * @return coordinates of the move
 */
Coord Command::getMovePos() const {
	return Board::getIndex(_str.substr(0, 2));
}

/**
 * Assuming the command represents a move, gets its lettter (tile to place on board).
 * @return move letter.
 */
char Command::getMoveLetter() const {
	return static_cast<char>(toupper(_str.at(3)));
}

/**
 * Checks whether the command represents a exchange action, ie. is of the form 'exchange <letter>'
 * @param forceToken - if not, a string containing (without a letter) is considered a exchange action.
 * @return true - command is a exchange action.
 * @return false - command is not a exchange action.
 */
bool Command::isExchange(bool forceToken) const {
	bool hasExchange = _str.find("exchange") != std::string::npos;

	if (!forceToken) {
		return hasExchange;
	}
	else return hasExchange
		&& _str.size() == std::string("exchange").size() + 2
		&& std::isalpha(_str.at(_str.size() - 1));
}

/**
 * Assuming the command is a exchange action, get its letter (to give the pool).
 * @return letter to give the pool.
 */
char Command::getExchangeLetter() const {
	return static_cast<char>(toupper(_str.at(_str.size() - 1)));
}

/**
 * Checks if intended command action is to check all players' hands.
 * @return true - command string contains "hand".
 * @return false - command string does not contain "hand".
 */
bool Command::isCheckHands() const {
	return _str.find("hand") != std::string::npos;
}

/**
 * Checks if intended command action is to spy on the pool.
 * @return true - command string contains "pool".
 * @return false - command string does not contain "pool".
 */
bool Command::isCheckPool() const {
	return _str.find("pool") != std::string::npos;
}

/**
 * Checks if intended command action is to check all players' scores.
 * @return true - command string contains "score" or "point"
 * @return false - command string does not contain "score" or "point".
 */
bool Command::isCheckScores() const {
	return _str.find("score") != std::string::npos
		|| _str.find("point") != std::string::npos;
}

/**
 * Checks if intended command action is to view in-game help message.
 * @return true - command string contains "help" or "tutorial".
 * @return false - command string does not contain "help" or "tutorial".
 */
bool Command::isHelp() const {
	return _str.find("help") != std::string::npos
		|| _str.find("tutorial") != std::string::npos;
}

/**
 * Checks if intended command action is to get a move hint.
 * @return true - command string contains "hint".
 * @return false - command string does not contain "hint".
 */
bool Command::isHint() const {
	return _str.find("hint") != std::string::npos;
}

/**
 * Checks if intended command action is to pass move.
 * @return true - command string contains "pass" or "skip".
 * @return false - command string does not contain "pass" or "skip".
 */
bool Command::isPass() const {
	return _str.find("pass") != std::string::npos
		|| _str.find("skip") != std::string::npos;
}

/**
 * Checks if intended command action is to erase command history.
 * @return true - command string contains "clear".
 * @return false - command string does not contain "clear".
 */
bool Command::isClear() const {
	return _str.find("clear") != std::string::npos;
}

/**
 * Checks is a command has conflicts, ie. more than one command keyword is in its string.
 * @return true - less than 2 command keywords were provided.
 * @return false - 2 or more command keywords provided.
 */
bool Command::hasNoConflicts() const {
	int count = 0;
	if (isExchange(false)) count++;
	if (isCheckHands()) count++;
	if (isCheckPool()) count++;
	if (isCheckScores()) count++;
	if (isHelp()) count++;
	if (isHint()) count++;
	if (isPass()) count++;
	if (isClear()) count++;
	return count < 2;
}

/**
 * Returns command string.
 * @return - Command string
 */
std::string Command::getCommandStr() const {
    return _str;
}