#include "Player.h"
#include "Move.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include "../common/ConsoleSetup.h"

Player::Player(Pool *pool, const std::string &name, int color) {
    int handSize = 7;
    _mayPass = 0;
    _score = 0;
    _color = color;
    _exchangeCount = 0;
    _name = name;
	stripSpaces(_name);
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

void Player::showHand(std::ostream &output, bool color) const {
    if (!getActualHandSize()) {
        output << "Nothing on hand\n";
        return;
    }
    for (const auto &i : _hand) {
        if (i == ' ') continue;
        else {
            if (color) outputBackForeColor(output, WHITE, _color, i);
            else output << i;
        }
        output << " ";
    }
    output << "\n";
}

void Player::addScore() {
    _score++;
}

bool Player::exchange(char letter, Pool* pool) {
    if (!pool->getCurrentSize()) return false;

    int handPos = getHandPosition(letter);
    if (!takeRandom(handPos, pool)) return false;

    pool->include(letter);
    _exchangeCount++;
    return true;
}

bool Player::takeRandom(int handPos, Pool *pool) {
    int poolSize = pool->getCurrentSize();
    int maxPos = _hand.size() - 1;

    if (handPos > maxPos || handPos < 0) return false;

    _hand.at(handPos) = ' '; //even if the pool is empty, remove from hand (endgame situations)
    if (!poolSize) return false;

    int randomPoolPos = randomBetween(0, poolSize - 1);
    _hand.at(handPos) = pool->getAllLetters().at(randomPoolPos);
    pool->take(randomPoolPos);
    return true;
}

void Player::resetExchangeCount() {
    _exchangeCount = 0;
}

int Player::getHandPosition(char letter) const {
    for (size_t i = 0; i < _hand.size(); ++i) {
        if (_hand.at(i) == letter) return i;
    }
    return -1;
}

bool Player::hasOnHand(char letter) const {
    for (const auto &i : _hand) {
        if (i == letter) return true;
    }
    return false;
}

bool Player::mayMove(const Board *board) const{
    coord pos = getPossibleMovePos(board);
    return pos.hCollumn != IMPOSSIBLE_MOVE_COORD && pos.vLine != IMPOSSIBLE_MOVE_COORD;
}

coord Player::getPossibleMovePos(const Board* board) const {
    coord boardDim = board->getDimensions();

    for (size_t line = 0; line < boardDim.vLine; ++line) {
        for (size_t col = 0; col < boardDim.hCollumn; ++col) {
            coord testPosition = { line,col };
            char letter = board->getLetters().at(line).at(col);
            const Move tryMove(testPosition, letter, board);

            if (!tryMove.hasProblems(this)) return testPosition;
        }
    }
    return { IMPOSSIBLE_MOVE_COORD,IMPOSSIBLE_MOVE_COORD };
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

int Player::getActualHandSize() const {
    int count = 0;
    for (const auto &letter : _hand) {
        if (letter != ' ') count++;
    }
    return count;
}