#include "Player.h"
#include <iostream>
#include "stringProcess.h"

Player::Player(Pool pool, std::string name) {
    int handSize = 7;
	_name = stripSpaces(name);
    _hand.resize(handSize);
    while (handSize--) {
        takeRandom(pool, handSize);
    }
}

int Player::getScore() const {
    return _score;
}

std::string Player::getName() const {
    return _name;
}

void Player::showHand() const {
    for (auto i : _hand) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

void Player::addScore(int score) {
    _score += score;
}

bool Player::exchange(int pos1, int pos2, Pool &pool) {
    if (pool.getCurrentSize() < 2) return false;
    char include1 = _hand.at(pos1);
    char include2 = _hand.at(pos2);
    if (!takeRandom(pool, pos1)) return false;
    if (!takeRandom(pool, pos2)) {
        pool.include(_hand.at(pos1)); //undo previous take
        return false;
    }
    pool.include(include1);
    pool.include(include2);
    return true;
}

bool Player::exchange(int pos1, Pool& pool) {
    if (pool.getCurrentSize() < 1) return false;
    char include = _hand.at(pos1);
    if (!takeRandom(pool, pos1)) return false;
    pool.include(include);
    return true;
}

bool Player::takeRandom(Pool &pool, int handPosition) {
    if (!pool.getCurrentSize()) return false;
    if (handPosition > (int) _hand.size()-1) return false;

    char random;
    do {
        random = 'A' + (rand() % pool.getAlphabetSize());
    } while (!pool.take(random));
    _hand.at(handPosition) = random;
    return true;
}