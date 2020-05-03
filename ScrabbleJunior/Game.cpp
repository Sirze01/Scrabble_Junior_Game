#include "Game.h"

Game::Game(Board* board, std::vector<std::string> playerNames) {
	_nPlayers = playerNames.size();
	_board = board;
	_pool = new Pool(board);
	_currentToMove = 0; //may change later, asking user

	for (int i = 0; i < _nPlayers;++i) {
		_players.push_back(new Player(_pool,playerNames.at(i)));
	}
}

Game::~Game() {
    delete _pool;
    for (int i = 0; i < _nPlayers;++i) {
        delete _players.at(i);
    }
}

void Game::askCommand() {
    Player *player = _players.at(_currentToMove);
    player->showHand();
    player->showScore();
    std::string input;
    for (;;) {
        std::cout << "\n" << player->getName() << ", make a move: ";
        std::getline(std::cin, input);
        Command command(input);
        if (command.isMove()) {
            Move move(&command, _board);
            if (move.hasProblems(player)) std::cout << "move has problems.\n";
            else {
                move.execute(player, _board, _pool);
                std::cout << "executing your beautiful move...\n";
                break;
            }
        }
        else if (command.isExchange()) {
            int token = command.getExchangeToken();
            bool success = true;
            if (isdigit(token) && !player->exchange(token, _pool)) success = false;
            else if (!isdigit(token) && !player->exchange((char)token, _pool)) success = false;

            if (!success) std::cout << "could not exchange! you have valid moves\n";
            else std::cout << "exchange successful\n";
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
        else if (command.isHint()) std::cout << "want an hint? buy premium\n"; //joke
        else std::cout << "command not recognized\n";
    }

    std::cout << "press enter.\n";
    std::cin.ignore(10000, '\n');
}

void Game::nextTurn() {
    _currentToMove++;
    if (_currentToMove == _nPlayers) _currentToMove = 0;
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