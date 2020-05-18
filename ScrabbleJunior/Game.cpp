#include "Game.h"

/**
 * A game directly changes a board, creates a pool based on it, and stores players to be iterated in each turn.
 * @param board - to changed during games.
 * @param playerNames - storing names of players by order.
 * @param playerColors - storing colors of players by order.
 * @param botFlags - storing isBot attribute of players by order.
 * @param firstToMove - position on the previous vectors of the player who shall go first.
 */
Game::Game(Board &board, const std::vector<std::string> &playerNames,
           const std::vector<int> &playerColors, const std::vector<bool> &botFlags, size_t firstToMove) :
        _board{board}, _pool{board}, _nPlayers{playerNames.size()},
        _playerColors{playerColors}, _currentPlayerPos{firstToMove}
{

    for (size_t i = 0; i < _nPlayers; ++i) {
        Player player = Player(_pool, playerNames.at(i), playerColors.at(i), botFlags.at(i));
        _players.push_back(player);
    }

    _currentPlayer = &_players.at(_currentPlayerPos);

    //card settings
    _compactCardView = _board.getDimensions().line < COMPACT_VIEW_MAX;
}


/**
 * Executes all commands following the game rules, including moves.
 * If the player is a bot, generates a move and shows a message.
 * If the player is human, gives access for commands and loops while the entered command is a valid move.
 * For both, if they can't move and it's the second turn, immediately returns; else clears console and shows board.
 * @param turnNumber - first or second turn.
 */
void Game::moveHandler(int turnNumber) {

    _currentPlayer->resetExchangeCount();
    bool ableToMove = _currentPlayer->mayMove(_board);

    if (_currentPlayer->getHasPassed()) {
        if (ableToMove) {
            _currentPlayer->doNotPass(); //can't pass if a move is passible
        } else if (turnNumber == 2) {
            return;
        }
    }

    showBoardAndCardView();
    std::stringstream coloredMessage;
    int playerColor = _currentPlayer->getColor();
    std::string name = _currentPlayer->getName();

    if (_currentPlayer->isBot()) {
        for (;;) { //exits when move is valid or exchanged once
            coloredMessage << "(turn " << turnNumber << ") ";
            if (ableToMove) {
                const Coord pos = _currentPlayer->getPossibleMovePos(_board);
                char letter = _board.getLetters().at(pos.line).at(pos.col);
                const Move move(pos, letter, _board);
                move.execute(*_currentPlayer, _board, _pool);
                coloredMessage << name << " will play " << Board::getPositionString(pos) << SPACE << letter
                               << ".\n";
                break;
            } else if (!_currentPlayer->getExchangeCount() && _currentPlayer->getHandSize() && _pool.getCurrentSize()) {
                int handPos = Util::randomBetween(0, _currentPlayer->getHandSize() - 1);
                char oldLetter = _currentPlayer->getHand().at(handPos);
                char newLetter = _currentPlayer->exchange(oldLetter, _pool);
                coloredMessage << name << " exchanged letter " << oldLetter << " from the pool and got letter "
                               << newLetter << ".\n\n";
                ableToMove = _currentPlayer->mayMove(_board);
                showPlayerInfo(true);
            } else { //exchange is not possible
                coloredMessage << name << " cannot move and thus will pass.\n";
                _currentPlayer->forcePass();
                break;
            }
        }
    } else {
        if (!ableToMove) { //if player has on hand and haven't passed last time, will have to pass manually
            if (_currentPlayer->getHasPassed()) {
                coloredMessage << "(turn " << turnNumber << ") "
                               << name << ", you still cannot move.\n" <<
                               "We will skip your turn automatically.\n";
            } else if (!_currentPlayer->getHandSize()) {
                _currentPlayer->forcePass();
                coloredMessage << "(turn " << turnNumber << ") "
                               << name << ", you have nothing on your hand.\n" <<
                               "Your turn will be skipped as long as you cannot move.\n";
            }
        }
    }

    std::string commandPrompt, input;

    for (;;) { //breaks when command is a valid move, or player passes
        std::stringstream regularMessage;

        if (Util::isEmpty(coloredMessage)) { //if colored message has content, it means user has no access to commands
            commandPrompt = "(turn " + std::to_string(turnNumber) + ") " + name + ": ";

            Util::paddingAndTopic(playerColor, true);
            std::cout << commandPrompt;

            std::getline(std::cin, input);
            Util::cleanBuffer();
            const Command command(input);
            if (command.isMove()) {
                const Move move(command, _board);
                int problemLevel = move.hasProblems(*_currentPlayer);
                if (problemLevel) {
                    regularMessage << "Could not play! ";
                    if (problemLevel == 1) {
                        regularMessage << "You are attempting to put a tile outside the board.\n";
                    } else if (problemLevel == 2) {
                        regularMessage << "The letter " << command.getMoveLetter() << " is not in that position.\n";
                    } else if (problemLevel == 3) {
                        regularMessage << "You do not have the letter " << command.getMoveLetter() << " on hand.\n";
                    } else if (problemLevel == 4) {
                        regularMessage << "That position already has a tile.\n";
                    } else if (problemLevel == 5) {
                        regularMessage << "You are not starting or continuing a word.\n";
                    }
                } else {
                    move.execute(*_currentPlayer, _board, _pool);
                    return;
                }
            } else if (command.hasNoConflicts()) {

                if (command.isExchange()) {
                    char token = command.getExchangeLetter();
                    if (ableToMove) {
                        regularMessage << "You can not exchange when you have possible moves. Pay attention!\n";
                    } else if (!_currentPlayer->hasOnHand(token)) {
                        regularMessage << "Could not exchange! You do not have the letter " << token
                                       << " on hand.\n";
                    } else if (!_pool.getCurrentSize()) {
                        _currentPlayer->forcePass();
                        coloredMessage << "The pool is empty.\n" <<
                                       "Your turn will be skipped as long as you cannot move.\n";
                    } else if (_currentPlayer->getExchangeCount()) {
                        _currentPlayer->forcePass();
                        coloredMessage << "You already exchanged once and still cannot move.\n" <<
                                       "Your turn will be skipped as long as you cannot move.\n";
                    } else {
                        char newLetter = _currentPlayer->exchange(token, _pool);
                        showPlayerInfo(true);
                        regularMessage << "The exchange was successful. You got letter " << newLetter
                                       << " from the pool.\n";
                        ableToMove = _currentPlayer->mayMove(_board);
                    }
                }
                else if (command.isCheckHands()) showPlayerInfo(true);
                else if (command.isCheckScores()) showPlayerInfo(false);
                else if (command.isCheckPool()) showPool();
                else if (command.isHelp()) showHelp();
                else if (command.isHint()) {
                    if (!ableToMove) {
                        regularMessage << "Maybe you can't move right now...\n";
                    } else {
                        const Coord pos = _currentPlayer->getPossibleMovePos(_board);
                        regularMessage << "Look carefully at the board on position "
                                       << Board::getPositionString(pos) << "...\n";
                    }
                } else if (command.isPass()) {
                    if (ableToMove) {
                        regularMessage << "You cannot pass when you have possible moves. Look carefully!\n";
                    } else if (_pool.getCurrentSize() && !_currentPlayer->getExchangeCount()) {
                        regularMessage << "The pool is not empty. Try to exchange a letter before passing.\n";
                    } else { //allow passing
                        _currentPlayer->forcePass();
                        return;
                    }
                } else if (command.isClear()) showBoardAndCardView();
                else regularMessage << Util::smartCommandAdvice(command.getCommandStr());

            } else
                regularMessage << "We found overlapping command keywords in your input. Type 'help' to learn why.\n";
        }

        if (!Util::isEmpty(regularMessage)) {
            Util::paddingAndTopic(WHITE, true);
            std::cout << regularMessage.str();
        }
        else if (!Util::isEmpty(coloredMessage)) {
            coloredMessage << "Press enter to continue.\n";
            std::string sentence;
            while (std::getline(coloredMessage, sentence)) {
                if (!sentence.empty()) {
                    Util::paddingAndTopic(playerColor, true);
                } else {
                    std::cout << "\n";
                }
                std::cout << sentence;
            }
            std::cout << "\n";
            Util::askEnter();
            return;
        }
    }
}


/** Makes the next player the current one. */
void Game::nextTurn() {
    _currentPlayerPos++;
    _currentPlayerPos %= _nPlayers;
    _currentPlayer = &_players.at(_currentPlayerPos);
}

/**
 * Checks if a game is finished - all tiles are highlighted, or all players cannot move and have passed
 * @return true 
 * @return false 
 */
bool Game::hasFinished() const {
    if (allHighlighted()) return true;
    for (const auto &player : _players) {
        if (!player.getHasPassed() || player.mayMove(_board)) return false;
    }
    return true;
}

/** Shows the board with scores card view. Output to the console the game winner and asks for to two enters. */
void Game::showEndMessage() const {
    showBoardAndCardView(false, false);

    int color = WHITE;
    int winner = getWinner();

    if (hasWinner()) color = _players.at(winner).getColor();

    Util::paddingAndTopic(WHITE, true);
    std::cout << "THE GAME HAS ENDED!\n";

    Util::paddingAndTopic(color, true);
    if (hasWinner()) std::cout << _players.at(winner).getName() << " won with brilliancy!\n";
    else std::cout << "There has been a draw! Congratulations to all.\n";

    Util::paddingAndTopic(WHITE, true);
    std::cout << "Press enter twice to exit.\n";
    int i = 2;
    while (i--) Util::askEnter();
}

/**
 * Shows the board and the card view - the info on the side of the board - which defaults to players.
 * @param hands - if true, show player's hands; else show scores.
 * @param turnInfo - whether to show "to play!" and "has passed" when appropriate
 */
void Game::showBoardAndCardView(bool hands, bool turnInfo) const {
    Util::clearConsole();
    _board.show();
    showPlayerInfo(hands, turnInfo);
}

/**
 * Puts player info in the card view (side of the board info)
 * @param hands - if true, show player's hands; else show scores.
 * @param turnInfo - whether to show "to play!" and "has passed" when appropriate
 */
void Game::showPlayerInfo(bool hands, bool turnInfo) const {
    std::stringstream toWrite;

    for (size_t i = 0; i < _nPlayers; ++i) {
        const Player player = _players.at(i);

        Util::outputForeColor(toWrite, player.getColor(), TOPIC);
        toWrite << player.getName();

        if (turnInfo) {
            if (i == _currentPlayerPos) toWrite << " - to play!";
            else if (player.getHasPassed()) toWrite << " - passed last turn";
        }
        toWrite << "\n";

        Util::outputForeColor(toWrite, player.getColor(), TOPIC);

        if (hands) {
            player.showHand(toWrite, i == _currentPlayerPos);
        } else {
            toWrite << player.getScore() << " points\n";
        }

        for (size_t j = 0; j < 5 - _nPlayers - _compactCardView; ++j) toWrite << "\n";
    }

    Util::writeCardView(_board.getDimensions().line, _board.getDimensions().col, toWrite);
}

/**
 * Check if the game has a winner (there are no two players with the same score)
 * @return true - winner found.
 * @return false - game has no winner.
 */
bool Game::hasWinner() const {
    return getWinner() != -1;
}

/**
 * Get game winner's position.
 * @return position if game has winner, else -1.
 */
int Game::getWinner() const {
    int maxScore = 0;
    int currentWinner = -1;
    for (size_t i = 0; i < _nPlayers; ++i) {
        int score = _players.at(i).getScore();
        if (score == maxScore) currentWinner = -1;
        else if (score > maxScore) {
            currentWinner = i;
            maxScore = score;
        }
    }
    return currentWinner;
}

/**
 * Check if all letters (non spaces) in the board are highlighted (have been played)
 * @return true - all tiles are highlighted.
 * @return false - at least one tile is available to be played.
 */
bool Game::allHighlighted() const {
    Coord boardDim = _board.getDimensions();
    std::vector<std::vector<char>> boardLetters = _board.getLetters();
    std::vector<std::vector<bool>> boardHighlights = _board.getHighlights();

    for (size_t line = 0; line < boardDim.line; ++line) {
        for (size_t col = 0; col < boardDim.col; ++col) {
            if (boardLetters.at(line).at(col) == SPACE) continue;
            else if (!boardHighlights.at(line).at(col)) return false;
        }
    }
    return true;
}

/** Makes the help message the card view. */
void Game::showHelp() const{
    std::stringstream toWrite;
    std::vector<std::string> intro =
            {
                    "Start or continue words with the tiles you have on hand.",
                    "You get one point for each word you complete.",
                    "Available commands:",
                    ""
            };

    std::vector<std::string> commands =
            {
                    "'move <Yx> <letter>' - play letter on position Yx.",
                    "'exchange <letter>' - exchange a letter from the pool.",
                    "'check hands' - have a look at all players' hands.",
                    "'check scores' - have a look at the current scores.",
                    "'check pool' - spy on the current state of the pool.",
                    "'get hint' - get some advice. Do not abuse of this!",
                    "'pass' - skip turn when you have no possible moves.",
                    "'clear' - erase command history and reload screen."
            };

    if (!_compactCardView) {
        for (const auto &sentence : intro) {
            toWrite << TOPIC << sentence << "\n";
        }
    }
    for (const auto &sentence : commands) {
        toWrite << TOPIC << ARROW << SPACE << sentence << "\n";
    }

    Util::writeCardView(_board.getDimensions().line, _board.getDimensions().col, toWrite);
}

/*
 * Makes pool letters the card view.
 * Mind that even if the player has the ability to spy on the pool, all takes from it are random.
 */
void Game::showPool() const {
    std::stringstream toWrite;

    std::vector<char> letters = _pool.getAllLetters();
    int size = letters.size();

    toWrite << TOPIC << size << " letters on the pool\n";

    const int MAX_LETTERS_PER_LINE = 10;
    for (int i = 0; i < size; ++i) {
        if (i % (MAX_LETTERS_PER_LINE + 1) == 0) {
            toWrite << "\n" << TOPIC;
        }
        toWrite << letters.at(i) << " ";
    }

    Util::writeCardView(_board.getDimensions().line, _board.getDimensions().col, toWrite);
}