#pragma once
#include "../common/Board.h"
class commandInterpreter {
public:
    commandInterpreter();
    commandInterpreter (std::string command);
    bool validation();
    void cmdHelp();
    bool cmdNew();
    bool cmdImport();
    bool cmdAdd();
    //bool cmdExport();
    //bool cmdExit();

private:
    std::string _command;
    std::string _modifiers;
    bool _state = false; // 0 to board closed, 1 to board open
    std::string _name;
};

