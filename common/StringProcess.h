#pragma once
#include <string>
#include <chrono>
#include <random>

int randomBetween(int lowerBound, int upperBound);

void lowerCase(std::string &command);
void stripSpaces(std::string &name);
void upperNameInitials(std::string &name);
void stripSpecialChars(std::string &name);
void stripCommandBloat(std::string& name);

std::string stringWriter(size_t text_width, std::string text, int padding);
std::string smartCommandAdvice(const std::string &command);

bool isAlpha(const std::string &toTest);
bool isDigit(const std::string &toTest);