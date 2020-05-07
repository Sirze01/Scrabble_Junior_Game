#pragma once
#include "../common/Board.h"
#include "../common/StringProcess.h"


class commandInterpreter {
public:
    commandInterpreter();
    commandInterpreter (std::string command);
    void edit(std::string command);
    std::string boardName();
    bool interpret(int &last);

    void cmdHelp();
    bool cmdNew();
    bool cmdImport();
    bool cmdAdd();
    bool cmdExport();

private:
    std::string _command;
    std::string _modifiers;

    bool _state = false; // 0 to board closed, 1 to board open
    std::string _name;
    Board _board;
};

