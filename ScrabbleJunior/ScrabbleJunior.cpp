#include <iostream>
#include <ctime>

#include "Pool.h"
#include "Player.h"
#include "PlayBoard.h"

void showPool(Pool pool) {
    std::map<char, int> my_letters = pool.getAllLetters();
    int count = 0;
    for (auto it = my_letters.cbegin(); it != my_letters.cend(); ++it) {
        std::cout << it->first << ": " << it->second << "\n";
        count += it->second;
    }
    std::cout << "\ntotal letters: " << count << "\n\n";
}

int main()
{
    srand(time(NULL)); //letter randomize

    //english alphabet 
    std::string englishAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::vector<char> alphabet(englishAlphabet.begin(), englishAlphabet.end());

    Pool my_pool(alphabet);
    showPool(my_pool);
    my_pool.exchange('Z', 'K');
    showPool(my_pool);
}