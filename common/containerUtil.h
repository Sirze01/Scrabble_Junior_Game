#pragma once

#include <string>
#include <chrono>
#include <random>

namespace Util {
	void lowerCase(std::string& str);
	void upperCase(std::string& str);
	void stripUnnecessarySpaces(std::string& str);
	void stripAllSpaces(std::string& str);
	void upperNameInitials(std::string& name);
	void stripSpecialChars(std::string& name, bool acceptDigits = false);
	void stripCommandBloat(std::string& name);

	void stringWriter(const std::string& text);
	std::string smartCommandAdvice(const std::string& command);

	bool isAlpha(const std::string& toTest, bool acceptSpaces = false);
	bool isDigit(const std::string& toTest);
	bool isEmpty(const std::stringstream& ss);

	void initRandom();
	int randomBetween(int lowerBound, int upperBound);
	void shuffle(std::vector<char>& v);
}