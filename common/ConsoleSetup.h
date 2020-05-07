#pragma once

#include <iostream>
#include <string>

//define UNICODE
#ifndef UNICODE
#define UNICODE
#endif

#ifdef _WIN32
#include <windows.h>
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

//card view
constexpr int
CARD_LEFT_PADDING = 12,
BOARD_LEFT_PADDING = 2,
BOARD_TOP_PADDING = 1;

//define 8-bit colors
constexpr int
RED = 88,
BLUE = 19,
GREEN = 22,
PINK = 127,
WHITE = 15,
BLACK = 0;

bool setupConsole();
void clearConsole();

void printBackColor(int backColor, const char c);
void printBackColor(int backColor, std::string message);
void printForeColor(int foreColor, const char c);
void printForeColor(int foreColor, std::string message);
void print(int foreColor, int backColor, const char c);
void print(int foreColor, int backColor, std::string message);

bool putCursorOnPos(int line, int col);
void eraseLineToTheEnd();
void eraseEntireLine();
void saveCurrentCursorPosition();
void restoreSavedCursorPosition();

void eraseCardView(int boardDimension, int col);