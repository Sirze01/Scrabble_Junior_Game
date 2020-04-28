#include <iostream>
#include <ctime>
#include <string>

#include "Pool.h"
#include "Player.h"
#include "../common/Board.h"
#include "Command.h"
#include "../common/Console_Setup.h"

int main()
{
    srand((unsigned int) time(NULL)); //letter randomize

    //english alphabet 
    std::string englishAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::vector<char> alphabet(englishAlphabet.begin(), englishAlphabet.end());

    Pool my_pool(alphabet);
    Board my_board("test.txt");
    Player player1(my_pool, "Alfredo");

    while (true) {
        std::string input;
        my_board.show(); std::cout << "\n";
        player1.showHand(); std::cout << "\n";
        my_pool.show();
        std::cout << "Input command: ";
        std::getline(std::cin, input);
        Command command(input);
        while (command.getCommand() != 1) {
            std::cout << "Not a move!!\n";
            Command command(input);
            std::cout << "Input command: ";
            std::getline(std::cin, input);
        }
        std::cout << "Moving...\n";
        player1.move(command, my_board, my_pool);
        std::cout << "Done, asking for another.\n";
    }
}