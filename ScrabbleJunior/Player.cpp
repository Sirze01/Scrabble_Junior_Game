#include "Player.h"
#include "../common/StringProcess.h"
#include "Move.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include "../common/ConsoleSetup.h"

//defined in Pool.cpp
extern unsigned SEED;
extern std::mt19937 RANDOM_GENERATOR;

Player::Player(Pool *pool, std::string name) {
    int handSize = 7;
    _score = 0;
    _colorName = RED_FORE;
	_name = stripSpaces(name);
    _hand.resize(handSize);
    pool->shuffle();
    while (handSize--) takeRandom(pool, handSize);
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

void Player::showScore() const {
    std::cout << _name << ": " << _score << " points\n";
}

void Player::addScore() {
    _score++;
}

bool Player::exchange(int handPos, Pool *pool) {
    char letter = _hand.at(handPos);
    if (!takeRandom(pool, handPos)) return false;
    pool->include(letter);
    return true;
}

bool Player::exchange(char letter, Pool* pool) {
    char handPos = getHandPosition(letter);
    if (!takeRandom(pool, handPos)) return false;
    pool->include(letter);
    return true;
}

bool Player::takeRandom(Pool *pool, int handPosition) {
    int poolSize = pool->getCurrentSize();
    if (!poolSize) return false;

    int maxPos = _hand.size() - 1;
    if (handPosition > maxPos || handPosition < 0) return false;

    //for shuffle purposes
    std::uniform_int_distribution<int> distribution{ 0, poolSize -1};
    int randomPoolPos = distribution(RANDOM_GENERATOR);

    _hand.at(handPosition) = pool->getAllLetters().at(randomPoolPos);
    pool->take(randomPoolPos);

    return true;
}

int Player::getHandPosition(char letter) const{
    int pos = -1;
    for (size_t i = 0; i < _hand.size(); ++i) {
        if (_hand.at(i) == letter) {
            pos = i;
            break;
        }
    }
    return pos;
}

char Player::getLetterOnHand(int handPosition) const {
    return _hand.at(handPosition);
}

bool Player::hasOnHand(char letter) const {
    int count = 0;
    for (auto i : _hand) {
        if (i == letter) return true;
    }
    return false;
}

bool Player::mayMove(const Board *board, const Pool *pool) const{
    coord boardDim = board->getDimensions();
    for (int line = 0; line < boardDim.vLine; ++line) {
        for (int col = 0; col < boardDim.hCollumn; ++col) {
            coord testPosition = { line,col };
            char letter = board->getLetters().at(line).at(col);
            Move tryMove(testPosition, letter, board);
            if (!tryMove.hasProblems(this)) {
                return true;
            }
        }
    }
    return false;
}