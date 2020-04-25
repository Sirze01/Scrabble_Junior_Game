#pragma once
#include <map>

class Pool {
public:
	Pool(std::vector<char> alphabet);
	bool take(char letter);
	bool exchange(char toInclude, char toTake);
private:
	std::map<char,int> _letters;
};

