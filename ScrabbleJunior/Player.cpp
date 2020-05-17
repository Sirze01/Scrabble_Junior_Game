#include "Player.h"
#include "Move.h"
#include <utility>

/**
 * A player must have a 7 letter hand with letters from the pool, and other attributes to be used in game.
 * @param pool - to take 7 letters. If it has less than 7 letters, will be empty afterwards.
 * @param name - to be displayed in the card view.
 * @param color - to assign highlights and others to him.
 * @param isBot - if true, will not have access to commands and will act automatically (see conditions in Game)
 */
Player::Player(Pool &pool, std::string name, int color, bool isBot):
    _name{std::move( name )}, _hasPassed{ false }, _score{ 0 }, _color{ color }, _exchangeCount{ 0 }, _isBot{ isBot }
{
    pool.shuffle();
    int handSize = 7;
    _hand.resize(handSize);
    while (handSize--) takeRandom(handSize,pool);
}

/**
 * Get player name.
 * @return name string.
 */
std::string Player::getName() const {
    return _name;
}

/**
 * Get player color.
 * @return 8-bit integer color.
 */
int Player::getColor() const {
    return _color;
}

/**
 * Get player's current score.
 * @return integer number of words completed until the moment.
 */
int Player::getScore() const {
    return _score;
}

/**
 * Write to the given stream letters in hand.
 * @param output - stream to be output to.
 * @param color - whether to use background color or not.
 */
void Player::showHand(std::ostream &output, bool color) const {
    if (!getHandSize()) {
        output << "Nothing on hand\n";
        return;
    }
    for (const auto &i : _hand) {
        if (color) Util::outputBackColor(output, _color, i);
        else output << i;
        output << SPACE;
    }
    output << "\n";
}

/** Increases player score by 1. */
void Player::addScore() {
    _score++;
}

/**
 * Include a letter in the pool, and take a random one from it.
 * @param letter - to be included in the pool.
 * @param pool - to take a random letter
 * @return 
 */
char Player::exchange(char letter, Pool& pool) {
    int handPos = getHandPosition(letter);
    if (handPos == -1) return '?';

    takeRandom(handPos, pool);
    pool.include(letter);
    _exchangeCount++;
    return _hand.at(handPos);
}

/**
 * Take a random letter from the pool and replaces given hand position with it. If pool is empty, erases hand position.
 * @param handPos - position of player's hand to put a pool letter in.
 * @param pool - to take a letter.
 * @return true - action successful.
 * @return false - hand position is invalid or pool is empty.
 */
bool Player::takeRandom(int handPos, Pool &pool) {
    if (-1 == handPos) return false;

    int poolSize = pool.getCurrentSize();
    if (!poolSize) { //if pool is empty, remove from hand (likely on endgame or very small boards)
        _hand.erase(_hand.begin() + handPos);
        return false;
    }

    int randomPoolPos = Util::randomBetween(0, poolSize - 1);
    _hand.at(handPos) = pool.getAllLetters().at(randomPoolPos);
    pool.take(randomPoolPos);
    return true;
}

/** Set _exchangeCount 0. */
void Player::resetExchangeCount() {
    _exchangeCount = 0;
}

/**
 * Get number of times player exchanged from the pool in the current turn.
 * @return _exchangeCount
 */
int Player::getExchangeCount() const {
    return _exchangeCount;
}

/**
 * Check if player has passed in the previous turn.
 * @return true - has passed
 * @return false - has not passed
 */
bool Player::getHasPassed() const {
    return _hasPassed;
}

/**
 * Get position of a letter in player's hand.
 * @param letter - to search for.
 * @return position at _hand if found, -1 if not.
 */
int Player::getHandPosition(char letter) const {
    for (size_t i = 0; i < _hand.size(); ++i) {
        if (letter == _hand.at(i)) return i;
    }
    return -1;
}

/**
 * Check if player has letter on hand.
 * @param letter - to search for.
 * @return true - letter found.
 * @return false - letter not found.
 */
bool Player::hasOnHand(char letter) const {
    return -1 != getHandPosition(letter);
}

/**
 * Check if the player may move (has at least a valid move) considering board and hand.
 * @param board - to evaluate letters.
 * @return true - if a move is possible.
 * @return false - if cannot move at the moment.
 */
bool Player::mayMove(const Board &board) const{
    coord pos = getPossibleMovePos(board);
    return pos.hColumn != IMPOSSIBLE_MOVE_COORD && pos.vLine != IMPOSSIBLE_MOVE_COORD;
}

/**
 * Get coordinates of the first board position the player can play into.
 * @param board - to evaluate letters.
 * @return board coordinates if a move is possible, else constant to be identified as error.
 */
coord Player::getPossibleMovePos(const Board& board) const {
    coord boardDim = board.getDimensions();
    std::vector<std::vector<char>> boardLetters = board.getLetters();

    for (size_t line = 0; line < boardDim.vLine; ++line) {
        for (size_t col = 0; col < boardDim.hColumn; ++col) {
            coord testPosition = { line,col };
            char letter = boardLetters.at(line).at(col);
            const Move moveToTest(testPosition, letter, board);

            if (!moveToTest.hasProblems(*this)) return testPosition;
        }
    }
    return { IMPOSSIBLE_MOVE_COORD,IMPOSSIBLE_MOVE_COORD };
}

/* Set _hasPassed to 1. */
void Player::forcePass() {
    _hasPassed = true;
}

/* Set _hasPassed to 0. */
void Player::doNotPass() {
    _hasPassed = false;
}

/* Get number of letters player has currently in hand. */
int Player::getHandSize() const {
    return _hand.size();
}

/* Get current player's hand. */
std::vector<char> Player::getHand() const {
    return _hand;
}

/* Check whether the player is a bot (moves automatically and has no access to commands). */
bool Player::isBot() const {
    return _isBot;
}
