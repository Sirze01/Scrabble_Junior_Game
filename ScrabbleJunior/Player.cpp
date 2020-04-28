#include "Player.h"
#include "Move.h"
#include <iostream>
#include "stringProcess.h"

Player::Player(Pool &pool, std::string name) {
    int handSize = 7;
	_name = stripSpaces(name);
    _hand.resize(handSize);
    while (handSize--) {
        takeRandom(pool, handSize);
        if (timesOnHand(_hand.at(handSize)) > 1) { //revert repeated letter take
            pool.include(_hand.at(handSize));
            handSize++;
        }
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

void Player::showScore() const {
    std::cout << _name << ": " << _score << " points\n";
}

void Player::addScore(int score) {
    _score += score;
}

bool Player::exchange(int pos1, Pool& pool) {
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

bool Player::move(Command command, Board& board, Pool &pool) {
    if (!command.isMove()) return false;
    Move my_move(command,board);
    if (!my_move.isValid(*this)) return false;

    board.highlight(command.getMovePos(board).vLine, command.getMovePos(board).hCollumn);
    takeRandom(pool, getHandPosition(command.getMoveLetter()));
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

int Player::timesOnHand(char letter) const {
    int count = 0;
    for (auto i : _hand) {
        if (i == letter) count++;
    }
    return count;
}

char Player::getLetterOnHand(int handPosition) const {
    return _hand.at(handPosition);
}

bool Player::hasOnHand(char letter) const {
    return timesOnHand(letter);
}