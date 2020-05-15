#pragma once
#include <string>
#include <chrono>
#include <random>

void lowerCase(std::string &command);
void stripSpaces(std::string &name);
void upperNameInitials(std::string &name);
void stripSpecialChars(std::string &name, bool acceptDigits = false);
void stripCommandBloat(std::string& name);

std::string stringWriter(size_t text_width, std::string text, int padding);
std::string smartCommandAdvice(const std::string &command);

bool isAlpha(const std::string &toTest, bool acceptSpaces = false);
bool isDigit(const std::string &toTest);