#include "Pool.h"
#include <iostream>
#include <random>
#include <algorithm>
#include "../common/ConsoleSetup.h"

//for shuffle purposes
unsigned SEED = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 RANDOM_GENERATOR(SEED);

Pool::Pool(const Board *board) {
	std::vector<std::vector<char>> boardContent = board->getLetters();

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

void Pool::shuffle() {
	//shuffle pool for next take
	std::shuffle(_letters.begin(), _letters.end(), RANDOM_GENERATOR);
}