#include <iostream>
#include <ctime>

#include "Pool.h"
#include "Player.h"
#include "PlayBoard.h"

int main()
{
    srand((unsigned int) time(NULL)); //letter randomize

    //english alphabet 
    std::string englishAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::vector<char> alphabet(englishAlphabet.begin(), englishAlphabet.end());

    Pool my_pool(alphabet);
    //my_pool.show();
    my_pool.show();

    Player p1(my_pool, "Alfredo Martins");
    p1.showHand();
    p1.takeRandom(my_pool, 4);
    std::cout << "took letter 4 from pool.\n";
    p1.showHand();
    std::cout << "exchanged first two with pool.\n";
    p1.exchange(0, 1, my_pool);
    p1.showHand();
    my_pool.show();
}