#include <iostream>
#include <string>

#include "Pool.h"
#include "Player.h"
#include "../common/Board.h"
#include "Command.h"
#include "../common/ConsoleSetup.h"
#include "Move.h"
#include "Game.h"

int main()
{
    setupConsole();

    Board my_board("test.txt");
    std::vector<std::string> playerNames = { "Alfredo Martins", "Comboios de Portugal"};
    Game my_game(&my_board, playerNames);

    for (;;) {
        my_board.show();
        std::cout << "\n";
        my_game.askMove();
        my_game.nextTurn();
        //clearConsole();
        if (my_game.hasFinished()) break;
        std::cout << "\n\n\n";
    }
}