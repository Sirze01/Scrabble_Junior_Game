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
    int nPlayers = 2;
    Board intro_board("intro_board.txt");
    intro_board.show();
    std::cout << "Welcome to our Scrabble Junior Game!\n";
    std::cout << "We hope you'll have a great time with us\n";
    std::cout << "For now, we will need some data to create a game.\n";
    std::cout << "\nNumber of players: ";
    while (!std::cin >> nPlayers || nPlayers > 4 || nPlayers < 2) {
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000,'\n');
        }
        std::cout << "Please input a number between 2 and 4!\n";
    }
    std::cin.ignore(10000, '\n');
    //return Game()
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