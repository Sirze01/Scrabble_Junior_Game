#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <functional>
#include <random>

//define UNICODE
#ifndef UNICODE
#define UNICODE
#endif

#ifdef _WIN32
#include <windows.h>
#undef max //include max buffer limit without issues
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

const char
TOPIC = '|',
SPACE = ' ',
ARROW = '>';

const int
CARD_LEFT_PADDING = 12,
BOARD_LEFT_PADDING = 2,
BOARD_TOP_PADDING = 1,
BOARD_BOTTOM_PADDING = 2,
BOARD_MIN_DIM = 7, //this is dependent on card view info cards
COMPACT_VIEW_MAX = 12,
MAX_PLAYER_NAME_SIZE = 20,
MAX_TEXT_WIDTH = 100;

//unfair game warning
const int
MAX_BOARD_LETTERS_WARNING = 4 * 7; //4 players, 7 letters in hand

//impossible move result
const size_t
IMPOSSIBLE_MOVE_COORD = SIZE_MAX;

const std::string
LEFT_PADDING_STR(BOARD_LEFT_PADDING, SPACE);

//define 8-bit colors
constexpr int
RED = 88,
BLUE = 19,
GREEN = 22,
PINK = 127,
ORANGE = 166,
DARK_GREY = 233,
WHITE = 15,
BLACK = 0;

namespace Util {
	bool setupConsole();
	void clearConsole();
	void cleanBuffer();
	void askEnter();

	void outputBackColor(std::ostream& toWrite, int backColor, const char c);
	void outputForeColor(std::ostream& toWrite, int foreColor, const char c);

	bool putCursorOnPos(int line, int col);
	void eraseLineToTheEnd();
	void eraseEntireLine();
	void saveCurrentCursorPosition();
	void restoreSavedCursorPosition();

	void writeCardView(int boardHeight, int boardWidth, std::stringstream& toWrite);
	void eraseCardView(int boardDimension, int col);

	void paddingAndTopic(int color, bool newLine = false);
}