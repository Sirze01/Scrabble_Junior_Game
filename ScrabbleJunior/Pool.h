#pragma once
#include <map>
#include <vector>

class Pool {
public:
	Pool(std::vector<char> alphabet);
	void show();
	bool take(char letter);
	bool include(char letter);
	std::map<char,int> getAllLetters() const;
	std::vector<char> getAlphabet() const;
	int getCurrentSize() const;
	int getAlphabetSize() const;
private:
	std::map<char,int> _letters;
	std::vector<char> _alphabet;
	int _currentSize;
	int _alphabetSize;
};

