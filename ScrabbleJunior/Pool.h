#pragma once
#include <vector>
#include "../common/Board.h"

class Pool {
public:
	Pool() = default;
	Pool(Board Board);
	void show();
	bool take(int pos);
	bool include(char letter);
	std::vector<char> getAllLetters() const;
	int getCurrentSize() const;
private:
	std::vector<char> _letters;
};