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

bool Player::move(Command command, Board& board, Pool &pool) {
    if (!(command.isMove())) return false;

    int vIndex = board.getIndex(command.getMove().at(0)).vLine;
    int hIndex = board.getIndex(command.getMove().at(0)).hCollumn;
    char letter = command.getMove().at(1).at(0);
    int handPosition = getHandPosition(letter);

    if (board.getLetters().at(vIndex).at(hIndex) != letter) return false;
    if (board.getHighlights().at(vIndex).at(hIndex) == 1) return false;
    if (handPosition == -1) return false;

    //start or continue word
    if (hIndex > 0) {
        if (board.getLetters().at(vIndex).at(hIndex - 1) != ' ') { //letter before at the line
            if (board.getHighlights().at(vIndex).at(hIndex - 1) == 0) return false;
        }
    }
    if (vIndex > 0) {
        if (board.getLetters().at(vIndex - 1).at(hIndex) != ' ') { //letter before at the collumn
            if (board.getHighlights().at(vIndex - 1).at(hIndex) == 0) return false;
        }
    }

    board.highlight(vIndex, hIndex);
    takeRandom(pool, handPosition);
    return true;
}

int Player::getHandPosition(char letter) const{
    int pos = -1;
    for (int i = 0; i < _hand.size(); ++i) {
        if (_hand.at(i) == letter) {
            pos = i;
            break;
        }
    }
    return pos;
}