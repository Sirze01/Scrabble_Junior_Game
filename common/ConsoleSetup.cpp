#include "../common/ConsoleSetup.h"
#include "../common/StringProcess.h"
#include <iostream>

bool setupConsole() {
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	// Set output mode to handle virtual terminal sequences
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hIn == INVALID_HANDLE_VALUE || hOut == INVALID_HANDLE_VALUE) {
		return false;
	}
	DWORD dwOriginalInMode = 0;
	DWORD dwOriginalOutMode = 0;
	if (!GetConsoleMode(hIn, &dwOriginalInMode)) {
		return false;
	}
	if (!GetConsoleMode(hOut, &dwOriginalOutMode)) {
		return false;
	}
	DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
	if (!SetConsoleMode(hIn, dwInMode))
	{
		// Failed to set VT input mode, can't do anything here.
		return false;
	}
	DWORD dwOutMode = dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	if (!SetConsoleMode(hOut, dwOutMode)) {
		// we failed to set both modes, try to step down mode gracefully.
		dwOutMode = dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwOutMode))
		{
			// Failed to set any VT mode, can't do anything here.
			return false;
		}
	}
#endif
	return true;
}

void clearConsole() {
	std::cout << "\x1B[2J\x1B[H";
}

void cleanBuffer() {
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void askEnter() {
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void outputBackColor(std::ostream& output, int backColor, const char c) {
	output << "\033[48;5;" << backColor << "m" << c << "\033[0m";
}

void outputForeColor(std::ostream& output, int foreColor, const char c) {
	output << "\033[38;5;" << foreColor << "m" << c << "\033[0m";
}

void outputBackForeColor(std::ostream& output, int foreColor, int backColor, const char c) {
	output << "\033[38;5;" << foreColor << ";48;5;" << backColor << "m" << c << "\033[0m";
}

bool putCursorOnPos(int line, int col) {
	if (line < 1 || col < 1) return false;
	std::cout << "\033[" << line << ";" << col << "H";
	return true;
}

void eraseLineToTheEnd() {
	std::cout << "\033[0K";
}

void eraseEntireLine() {
	std::cout << "\033[2K";
}

void saveCurrentCursorPosition() {
	std::cout << "\0337";
}

void restoreSavedCursorPosition() {
	std::cout << "\0338";
}

void eraseCardView(int boardDimension, int col) {
	saveCurrentCursorPosition();
	if (boardDimension < BOARD_MIN_DIM) boardDimension = BOARD_MIN_DIM;
	boardDimension += BOARD_TOP_PADDING + 2; //2 frames
	for (int i = 1; i <= boardDimension; i++) {
		putCursorOnPos(i, col);
		eraseLineToTheEnd();
	}
	restoreSavedCursorPosition();
}

void writeCardView(int boardHeight, int boardWidth, std::stringstream &toWrite){
	saveCurrentCursorPosition();
	int line = 2 + BOARD_TOP_PADDING;
	int col = 2 * boardWidth + CARD_LEFT_PADDING;
	eraseCardView(boardHeight, col);

	std::string current;
	while (getline(toWrite, current)) {
		putCursorOnPos(line++, col);
		std::cout << current;
	}

	restoreSavedCursorPosition();
}

void paddingAndTopic(int color, bool newLine) {
	if (newLine) std::cout << std::endl;
	std::cout << LEFT_PADDING_STR;
	outputForeColor(std::cout, color, TOPIC);
}