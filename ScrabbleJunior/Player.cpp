#include "Player.h"
#include "../common/StringProcess.h"
#include "Move.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include "../common/ConsoleSetup.h"

extern unsigned SEED;
extern std::mt19937 RANDOM_GENERATOR;

Player::Player(Pool *pool, std::string name, int color) {
    int handSize = 7;
    _score = 0;
    _color = color;
    _exchangeCount = 0;
	_name = stripSpaces(name);
    _hand.resize(handSize);
    pool->shuffle();
    while (handSize--) takeRandom(handSize,pool);
}

int Player::getScore() const {
    return _score;
}

std::string Player::getName() const {
    return _name;
}

void Player::showHand(bool color) const {
    if (!getHandSize()) {
        std::cout << "Nothing on hand";
        return;
    }
    for (auto i : _hand) {
        if (i == ' ') continue;
        else {
            if (color) print(WHITE, _color, i);
            else std::cout << i;
        }
        std::cout << " ";
    }
    std::cout << "\n";
}

void Player::addScore() {
    _score++;
}

bool Player::exchange(char letter, Pool* pool) {
    if (!pool->getCurrentSize()) return false;

    char handPos = getHandPosition(letter);
    if (!takeRandom(handPos, pool)) return false;

    pool->include(letter);
    _exchangeCount++;
    return true;
}

bool Player::takeRandom(int handPos, Pool *pool) {
    int poolSize = pool->getCurrentSize();
    int maxPos = _hand.size() - 1;

    if (handPos > maxPos || handPos < 0) return false;

    _hand.at(handPos) = ' ';
    if (!poolSize) return false;


    //for shuffle purposes
    std::uniform_int_distribution<int> distribution{ 0, poolSize -1};
    int randomPoolPos = distribution(RANDOM_GENERATOR);

    _hand.at(handPos) = pool->getAllLetters().at(randomPoolPos);
    pool->take(randomPoolPos);

    return true;
}

void Player::resetExchangeCount() {
    _exchangeCount = 0;
}

int Player::getHandPosition(char letter) const {
    for (size_t i = 0; i < _hand.size(); ++i) {
        if (_hand.at(i) == letter) {
            return i;
        }
    }
    return -1;
}

bool Player::hasOnHand(char letter) const {
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

coord Player::getPossiblePos(const Board* board, const Pool* pool) const {
    if (!mayMove(board, pool)) return { -1,-1 };

    coord boardDim = board->getDimensions();

    for (int line = 0; line < boardDim.vLine; ++line) {
        for (int col = 0; col < boardDim.hCollumn; ++col) {
            coord testPosition = { line,col };
            char letter = board->getLetters().at(line).at(col);
            Move tryMove(testPosition, letter, board);
            if (!tryMove.hasProblems(this)) {
                return testPosition;
            }
        }
    }
    return { -1,-1 };
}

int Player::getColor() const {
    return _color;
}

int Player::getExchangeCount() const {
    return _exchangeCount;
}

bool Player::getMayPass() const {
    return _mayPass;
}

void Player::forcePass() {
    _mayPass = true;
}

void Player::doNotPass() {
    _mayPass = false;
}

int Player::getHandSize() const {
    int count = 0;
    for (auto letter : _hand) {
        if (letter != ' ') count++;
    }
    return count;
}