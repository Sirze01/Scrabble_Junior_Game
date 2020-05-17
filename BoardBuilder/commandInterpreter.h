#pragma once
#include "../common/Board.h"



class commandInterpreter {
public:
    commandInterpreter (std::vector<std::string> &dict, std::string &boardName, Board &board,
                        bool &dictOpen, bool &boardOpen, std::string command);
    void interpret(int &statusCodes);

private:
    std::string _command;
    std::string _modifiers;
    bool& _dictOpen;
    bool& _boardOpen; // 0 to board closed, 1 to board open
    std::vector<std::string>& _dict;
    std::string& _name;
    Board& _board;

    void importPath();
    void boardName();
    bool cmdDict();
    static void cmdHelp();
    bool cmdNewBoard();
    bool cmdImportBoard();
    bool cmdAdd();
    bool cmdRemove();
    bool cmdExport() const;
    void cmdDelete(int &statusCodes);
    static void cmdExit(int &statusCodes);

};

