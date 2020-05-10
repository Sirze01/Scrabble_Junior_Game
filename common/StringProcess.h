#pragma once
#include <string>

std::string lowerCase(std::string command);
std::string stripSpaces(std::string name);
std::string upperNameInitials(std::string name);
std::string stripSpecialChars(std::string name);
std::string stringWriter(int text_width, std::string text, int padding);
std::string stripCommandBloat(std::string name);
std::string smartCommandAdvice(std::string command);
bool isAlpha(std::string toTest);
