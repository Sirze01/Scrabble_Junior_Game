#pragma once

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <wincon.h>
#endif

//define UNICODE
#ifndef UNICODE
#define UNICODE
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

bool SetupConsole() {
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

/**Clears the console*/
void clearConsole(){
    std::cout << "\x1B[2J\x1B[H";
}
