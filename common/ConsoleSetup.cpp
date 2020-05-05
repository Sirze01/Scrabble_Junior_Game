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

void printBackColor(int backColor, const char c) {
    std::cout << "\x1b[1m";
    std::cout << "\033[48;5;" << backColor << "m" << c << "\033[0m";
}

void printBackColor(int backColor, std::string message) {
    std::cout << "\x1b[1m";
    std::cout << "\033[48;5;" << backColor << "m" << message << "\033[0m";
}

void printForeColor(int foreColor, const char c) {
    std::cout << "\x1b[1m";
    std::cout << "\033[38;5;" << foreColor << "m" << c << "\033[0m";
}

void printForeColor(int foreColor, std::string message) {
    std::cout << "\x1b[1m";
    std::cout << "\033[38;1;" << foreColor << "m" << message << "\033[0m";
}

void print(int foreColor, int backColor, const char c) {
    std::cout << "\x1b[1m";
    std::cout << "\033[38;5;" << foreColor << ";48;5;" << backColor << "m" << c << "\033[0m";
}

void print(int foreColor, int backColor, std::string message) {
    std::cout << "\x1b[1m";
    std::cout << "\033[38;5;" << foreColor << ";48;5;" << backColor << "m" << message << "\033[0m";
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