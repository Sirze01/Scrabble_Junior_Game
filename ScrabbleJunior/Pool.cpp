#include "Pool.h"

/**
 * The pool contais all non-space chars of the board (ie. which can be played in the game).
 * @param board - to check chars.
 */
Pool::Pool(const Board &board) : _letters{board.getNonEmptyChars()} {}

/**
 * Remove a letter from the pool. Updates size of the container.
 * @param pos - position of _letters to be erased
 */
void Pool::take(int pos) {
	_letters.erase(_letters.begin() + pos);
}

/** Shuffle pool letters. */
void Pool::shuffle() {
	Util::shuffle(_letters);
}

/**
 * Include a letter in the pool.
 * @param letter - char to be appended to _letters
 */
void Pool::include(char letter) {
	_letters.push_back(letter);
}

/**
 * Get pool chars.
 * @return vector _letters
 */
std::vector<char> Pool::getAllLetters() const {
	return _letters;
}

/**
 * Get size of the pool.
 * @return size of the vector _letters.
 */
int Pool::getCurrentSize() const {
	return _letters.size();
}