#include "../common/consoleUtil.h"
#include "../common/containerUtil.h"

/**
 * If on Windows, tries to enable console support for ANSI escape codes. Else does nothing.
 * @return true - on Windows: the activation was successful; on Linux: it's a placeholder (unix has native support)
 * @return false - some error ocurred (eg. Windows version before 2016).
 * Note that in these case the program will misbehave since we use ANSI escape codes extensively.
 */
bool Util::setupConsole() {
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

/** Clears the console. */
void Util::clearConsole() {
	std::cout << "\x1B[2J\x1B[H";
}

/** In case of input fail, the fail flag is cleared and all characters in buffers are ignored. */
void Util::cleanBuffer() {
	if (std::cin.fail() || std::cin.eof()) { //eof will require a extra input, but program won't loop infinitely
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

/** Ignores all characters in the input buffer.
 If there are no characters to ignore, asks some input to discard, which is why we use as ask enter. */
void Util::askEnter() {
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * Outputs to the stream a char with given background color.
 * @param output - stream to write to
 * @param backColor - color to use
 * @param c - char to print
 */
void Util::outputBackColor(std::ostream& output, int backColor, const char c) {
	output << "\033[48;5;" << backColor << "m" << c << "\033[0m";
}

/**
 * Outputs to the stream a char with given foreground color.
 * @param output - stream to write to
 * @param foreColor - color to use
 * @param c - char to print
 */
void Util::outputForeColor(std::ostream& output, int foreColor, const char c) {
	output << "\033[38;5;" << foreColor << "m" << c << "\033[0m";
}

/**
 * Put cursor on given console position. Mind that console coordinates start at (1,1).
 * @param line - x coordinate.
 * @param col - y coordinate.
 * @return true - given coordinates were valid.
 * @return false - coordinates invalid, aborted operation
 */
bool Util::putCursorOnPos(size_t line, size_t col) {
	if (line == 0 || col == 0) return false;
	std::cout << "\033[" << line << ";" << col << "H";
	return true;
}

/** Blanks the current line, beggining at the current cursor position and ending at the end of the line. */
void Util::eraseLineToTheEnd() {
	std::cout << "\033[0K";
}

/** Save the current cursor position to later restore. */
void Util::saveCurrentCursorPosition() {
	std::cout << "\033" << "7";
}

/** Restore previously saved cursor position. */
void Util::restoreSavedCursorPosition() {
	std::cout << "\033" << "8";
}

/**
 * Blanks each line at the side of the board.
 * @param boardHeight - to check on which line to begin, and to end.
 * @param col - x coordinate from which to erase each line until the end.
 */
void Util::eraseCardView(size_t boardHeight, size_t col) {
	saveCurrentCursorPosition();
	if (boardHeight < BOARD_MIN_DIM) boardHeight = BOARD_MIN_DIM;
	boardHeight += BOARD_TOP_PADDING + 2; //2 frames
	for (size_t i = 1; i <= boardHeight; i++) {
		putCursorOnPos(i, col);
		eraseLineToTheEnd();
	}
	restoreSavedCursorPosition();
}

/**
 * Erase the card view area (on the side of the board) and write on it the content of the given stream.
 * @param boardHeight - to get number of lines to erase before writing.
 * @param boardWidth - to get starting column (after adding padding).
 * @param toWrite - stream which is content is to be output to the console.
 */
void Util::writeCardView(size_t boardHeight, size_t boardWidth, std::stringstream &toWrite){
	saveCurrentCursorPosition();

	size_t line = CARD_TOP_PADDING + BOARD_TOP_PADDING;
	size_t col = CARD_TOP_PADDING * boardWidth + CARD_LEFT_PADDING;
	eraseCardView(boardHeight, col);

	std::string current;
	while (getline(toWrite, current)) {
		putCursorOnPos(line++, col);
		std::cout << current;
	}

	restoreSavedCursorPosition();
}

/**
 * Output to the console the default padding and the TOPIC character.
 * @param color - color of the TOPIC character to be printed
 * @param newLine - whether to add a new line before.
 */
void Util::paddingAndTopic(int color, bool newLine) {
	if (newLine) std::cout << std::endl;
	std::cout << LEFT_PADDING_STR;
	outputForeColor(std::cout, color, TOPIC);
}