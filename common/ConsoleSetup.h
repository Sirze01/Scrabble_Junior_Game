#pragma once

#include <iostream>
#include <string>

//define UNICODE
#ifndef UNICODE
#define UNICODE
#endif

#ifdef _WIN32
#include <windows.h>
#include <wincon.h>
#endif

#ifdef _WIN32
// Fix compilatin on MinGW
#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008
#endif

#ifndef ENABLE_VIRTUAL_TERMINAL_INPUT
#define ENABLE_VIRTUAL_TERMINAL_INPUT 0x0200
#endif

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#endif

//define colors
constexpr int RED_BACK = 41, RED_FORE = 31, BLUE_BACK = 44, BLUE_FORE = 34,
YELLOW_BACK = 43, YELLOW_FORE = 33, GREEN_BACK = 42, GREEN_FORE = 32,
DEFAULT_BACK = 49, DEFAULT_FORE = 39, BOLD = 1, UNDERLINE = 4, ALL_DEFAULTS = 0;

bool setupConsole();
void clearConsole();
void print(const char message, const int foreColor, const int backColor);
void print(std::string message, int foreColor);