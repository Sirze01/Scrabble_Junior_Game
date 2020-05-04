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
    std::vector<std::string> playerNames = { "Alfredo Martins", "Comboios de Portugal", "Nunca Sentado"};
    std::vector<int> playerColors = { BLUE,RED,GREEN };
    Game my_game(&my_board, playerNames, playerColors, 0);

    for (;;) {
        my_board.show();
        my_game.askCommand();
        clearConsole();

        my_board.show();
        my_game.askCommand();

        my_game.nextTurn();
        clearConsole();
        if (my_game.hasFinished()) break;
    }

    clearConsole();
    if (my_game.hasWinner()) {
        std::cout << my_game.getWinner() << "won! Final classifications:\n";
        my_game.showScores();
    }
    else std::cout << "There has been a draw! Congrats to all.\n";
}