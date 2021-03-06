#pragma once

#include "../common/Board.h"

class Pool {
public:
	explicit Pool(const Board &Board);
	void take(int pos);
	void include(char letter);
	void shuffle();
	std::vector<char> getAllLetters() const;
	int getCurrentSize() const;
private:
	std::vector<char> _letters;
};