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
    bool _dictBool = false;
    bool _state = false; // 0 to board closed, 1 to board open
private:
    std::string _command;
    std::string _modifiers;

    std::vector<std::string> _dict;
    std::string _name;
    Board _board;
    void cmdHelp();
    bool cmdDict();
    bool cmdNew();
    bool cmdImport();
    bool cmdAdd(int &last);
    bool cmdExport();
    void cmdDelete(int &last);
    void cmdExit(int &last);
};

