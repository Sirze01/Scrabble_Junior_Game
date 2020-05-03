#include "Game.h"

Game::Game(Board* board, std::vector<std::string> playerNames, int firstToMove) {
	_nPlayers = playerNames.size();
	_board = board;
	_pool = new Pool(board);

	for (int i = 0; i < _nPlayers;++i) {
		_players.push_back(new Player(_pool,playerNames.at(i)));
	}

    _currentPlayerPos = firstToMove % _nPlayers;
    _currentPlayer = _players.at(_currentPlayerPos);
}

Game::~Game() {
    delete _pool;
    for (int i = 0; i < _nPlayers;++i) {
        delete _players.at(i);
    }
}

void Game::askCommand() {
    _currentPlayer->showHand();
    _currentPlayer->showScore();
    std::string input;
    for (;;) {
        std::cout << "\n" << _currentPlayer->getName() << " to play: ";
        std::getline(std::cin, input);
        Command command(input);
        if (command.isMove()) {
            Move move(&command, _board);
            if (move.hasProblems(_currentPlayer)) std::cout << "move has problems.\n";
            else {
                move.execute(_currentPlayer, _board, _pool);
                std::cout << "executing your beautiful move...\n";
                break;
            }
        }
        else if (command.isExchange()) {
            int token = command.getExchangeLetter();
            if (!_currentPlayer->exchange((char)token, _pool)) std::cout << "could not exchange!\n";
            else {
                std::cout << "exchange successful\n";
                _currentPlayer->showHand();
            }
        }
        else if (command.isCheckHands()) {
            for (auto player : _players) {
                std::cout << player->getName() << std::endl;
                player->showHand();
                std::cout << std::endl;
            }
        }
        else if (command.isCheckPool()) _pool->show();
        else if (command.isHelp()) std::cout << "life is short. enjoy yourself\n"; //joke
        else if (command.isHint()) {
            coord pos = _currentPlayer->getPossiblePos(_board,_pool);
            if (pos.hCollumn == -1 || pos.vLine == -1) std::cout << "I'm afraid you can't move right now...\n";
            else std::cout << "Look carefully at the board on position " << (char)('A' + pos.vLine) << (char)('a' + pos.hCollumn) << "...\n";
        }
        else std::cout << "command not recognized\n";
    }

    std::cout << "press enter.\n";
    std::cin.ignore(10000, '\n');
}

void Game::nextTurn() {
    _currentPlayerPos++;
    if (_currentPlayerPos == _nPlayers) _currentPlayerPos = 0;
    _currentPlayer = _players.at(_currentPlayerPos);
}

bool Game::hasFinished() const {
    Board board = *_board;
    int maxLine = board.getDimensions().vLine - 1;
    int maxCol = board.getDimensions().hCollumn - 1;

    for (int line = 0; line <= maxLine;++line) {
        for (int col = 0;col <= maxCol;++col) {
            if (board.getLetters().at(line).at(col) != ' ') {
                if (!board.getHighlights().at(line).at(col)) return false;
            }
        }
    }
    return true;
}

int Game::getWinner() const {
    if (!hasFinished()) return -1;
    int maxScore = 0; int currentWinner = -1;
    for (int i = 0; i < _nPlayers;++i) {
        int score = _players.at(i)->getScore();
        if (score == maxScore) currentWinner = -1;
        else if (score > maxScore) currentWinner = i;
    }
    return currentWinner;
}