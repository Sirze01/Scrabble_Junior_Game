#include "Pool.h"
#include <iostream>

Pool::Pool(std::vector<char> alphabet) {
	_alphabet = alphabet;
	_currentSize = 0;
	_alphabetSize = alphabet.size();
	unsigned min = 3;

	//initialize map
	for (auto i : alphabet) {
		_letters.insert(std::pair<char, int>(i, min));
		_currentSize += min;
	}

	//increment number of letters
	int remaining = 101 - min*_alphabetSize; char random;
	while (remaining--) {
		random = 'A' + (rand() % _alphabetSize);
		_letters.at(random)++;
		_currentSize++;
	}
}

void Pool::show() {
	std::map<char, int> my_letters = getAllLetters();
	int count = 0;
	for (auto it = my_letters.cbegin(); it != my_letters.cend(); ++it) {
		std::cout << it->first << ": " << it->second << "\n";
		count += it->second;
	}
	std::cout << "total letters: " << count << "\n";
	std::cout << "current size: " << getCurrentSize() << "\n";
}

bool Pool::take(char letter) {
	if (_letters.find(letter) != _letters.end()) {
		if (_letters.at(letter)) {
			_letters.at(letter)--;
			_currentSize--;
			return true;
		}
	}
	return false;
}

bool Pool::include(char letter) {
	if (_letters.find(letter) != _letters.end()) {
		_letters.at(letter)++;
		_currentSize++;
		return true;
	}
	return false;
}

std::map<char,int> Pool::getAllLetters() const {
	return _letters;
}

std::vector<char> Pool::getAlphabet() const {
	return _alphabet;
}

int Pool::getCurrentSize() const {
	return _currentSize;
}

int Pool::getAlphabetSize() const {
	return _alphabetSize;
}