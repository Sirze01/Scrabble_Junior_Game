#pragma once
#include <vector>
#include "../common/Board.h"
#include <chrono>
#include <random>

int randomBetween(int lowerBound, int upperBound);

class Pool {
public:
	Pool() = default;
	Pool(const Board &Board);
	void take(int pos);
	void include(char letter);
	void shuffle();
	std::vector<char> getAllLetters() const;
	int getCurrentSize() const;
private:
	std::vector<char> _letters;
};