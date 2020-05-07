#include <iostream>
#include <string>

#include "Pool.h"
#include "Player.h"
#include "../common/Board.h"
#include "Command.h"
#include "../common/ConsoleSetup.h"
#include "Move.h"
#include "Game.h"



void createGame() {
    Board intro_board("intro_board.txt");
    intro_board.show();
    std::cout << "Welcome! How many players";
}

int main()
{
    setupConsole();
    createGame();
    std::cin.ignore();
  
    Board my_board("test.txt");
    std::vector<std::string> playerNames = { "Alfredo Martins", "Comboios de Portugal","Nunca Sentado"};
    std::vector<int> playerColors = { GREEN,RED,BLUE};
    Game my_game(&my_board, playerNames, playerColors, 0);

    for (;;) {
        my_game.askCommand(1);
        my_game.askCommand(2);
        my_game.nextTurn();
        if (my_game.hasFinished()) break;
    }

    clearConsole();
    if (my_game.hasWinner()) {
        std::cout << my_game.getWinner() << "won! Final classifications:\n";
        my_game.showScores();
    }
    else std::cout << "There has been a draw! Congrats to all.\n";
}