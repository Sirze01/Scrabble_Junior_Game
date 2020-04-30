#include <iostream>
#include <ctime>
#include <string>

#include "Pool.h"
#include "Player.h"
#include "../common/Board.h"
#include "Command.h"
#include "../common/ConsoleSetup.h"
#include "Move.h"

int main()
{
#ifdef _WIN32
    setupConsole();
#endif
    srand((unsigned int) time(NULL)); //letter randomize

    Board my_board("test.txt");
    Pool my_pool(my_board);
    Player player1(my_pool, "Alfredo");
    my_pool.show();

    for (;;) {
        my_board.show();
        std::cout << "\n";
        player1.showHand();
        player1.showScore();
        player1.mayMove(my_board,my_pool);
        std::string input;
        std::cout << "\nMove: ";
        std::getline(std::cin, input);
        Command command(input);
        if (command.getCommand() == 2) {
            if (player1.mayMove(my_board,my_pool)) std::cout << "Oops! You have a valid move, so you can not exchange from pool.\n";
            else {
                player1.exchange(1, my_pool);
                std::cout << "exchanging from pool...\n";
            }
        }
        else if (command.isMove()) {
            Move move(command, my_board);
            if (!move.hasProblems(player1)) move.execute(player1, my_board, my_pool);
            else std::cout << "move has problems.\n";
        }
        else std::cout << "Not recognized or implemented yet.\n";
        std::cout << "press enter.\n";
        std::cin.ignore(10000,'\n');
        clearConsole();
    }
}