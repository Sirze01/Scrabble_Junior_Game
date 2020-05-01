#include "Pool.h"
#include <iostream>

Pool::Pool(Board board) {
	std::vector<std::vector<char>> boardContent = board.getLetters();

	//add letters in board
	for (std::vector<char> v : boardContent) {
		for (char c : v) {
			if (c != ' ') {
				_letters.push_back(c);
			}
		}
	}
}

void Pool::show() {
	std::vector<char> my_letters = getAllLetters();
	for (auto i : my_letters) std::cout << i << " ";
	std::cout << std::endl;
	std::cout << "pool size: " << getCurrentSize() << "\n";
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