#pragma once
#include <vector>
#include "../common/Board.h"
#include <chrono>
#include <random>

class Pool {
public:
	Pool() = default;
	Pool(Board Board);
	void show();
	bool take(int pos);
	bool include(char letter);
	void shuffle();
	std::vector<char> getAllLetters() const;
	int getCurrentSize() const;
private:
	std::vector<char> _letters;
};