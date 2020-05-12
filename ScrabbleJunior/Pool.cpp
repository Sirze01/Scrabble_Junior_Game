#include "Pool.h"
#include <iostream>
#include <random>
#include <algorithm>
#include "../common/ConsoleSetup.h"

extern unsigned SEED;
extern std::mt19937 RANDOM_GENERATOR;

Pool::Pool(const Board *board) {
	_letters = board->getNonEmptyChars();
}

bool Pool::take(int pos) {
	if (pos > getCurrentSize()) return false;
	else _letters.erase(_letters.begin() + pos);
	return true;
}

bool Pool::include(char letter) {
	_letters.push_back(letter);
	return true;
}

std::vector<char> Pool::getAllLetters() const {
	return _letters;
}

int Pool::getCurrentSize() const {
	return _letters.size();
}

void Pool::shuffle() {
	std::shuffle(_letters.begin(), _letters.end(), RANDOM_GENERATOR);
}