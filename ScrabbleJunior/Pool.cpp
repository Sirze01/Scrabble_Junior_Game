#include "Pool.h"
#include <iostream>
#include <random>
#include <algorithm>
#include "../common/ConsoleSetup.h"

extern unsigned const SEED;
extern std::mt19937 RANDOM_GENERATOR;

int randomBetween(int lowerBound, int upperBound) {
	std::uniform_int_distribution<int> distribution{ lowerBound, upperBound };
	return distribution(RANDOM_GENERATOR);
}

Pool::Pool(const Board &board) : _letters{board.getNonEmptyChars()} {}

void Pool::take(int pos) {
	_letters.erase(_letters.begin() + pos);
}

void Pool::include(char letter) {
	_letters.push_back(letter);
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