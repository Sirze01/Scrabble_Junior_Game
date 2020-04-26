#include "Pool.h"

Pool::Pool(std::vector<char> alphabet) {
	_alphabet = alphabet;
	unsigned min = 3;

	//initialize map
	for (auto i : alphabet) {
		_letters.insert(std::pair<char, int>(i, min));
	}

	//increment number of letters
	int remaining = 101 - min*alphabet.size(); char random;
	while (remaining--) {
		random = 'A' + (rand() % alphabet.size());
		_letters.at(random)++;
	}
}

bool Pool::take(char letter) {
	if (_letters.find(letter) != _letters.end()) {
		if (_letters.at(letter)) {
			_letters.at(letter)--;
			return true;
		}
	}
	return false;
}

bool Pool::include(char letter) {
	if (_letters.find(letter) != _letters.end()) {
		_letters.at(letter)++;
		return true;
	}
	return false;
}

bool Pool::exchange(char toInclude, char toTake) {
	if (!this->take(toTake)) return false;
	if (!this->include(toInclude)) return false;
	return true;
}

std::map<char,int> Pool::getAllLetters() const {
	return _letters;
}

std::vector<char> Pool::getAlphabet() const {
	return _alphabet;
}