#pragma once
#include <map>
#include <vector>

class Pool {
public:
	Pool(std::vector<char> alphabet);
	bool take(char letter);
	bool include(char letter);
	bool exchange(char toInclude, char toTake);
	std::map<char,int> getAllLetters() const;
	std::vector<char> getAlphabet() const;
private:
	std::map<char,int> _letters;
	std::vector<char> _alphabet;
};

