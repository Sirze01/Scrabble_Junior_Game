#include "Game.h"

Game::Game(Board* board, std::vector<std::string> playerNames) {
	_nPlayers = playerNames.size();
	_board = board;
	_pool = new Pool(*board);
	_currentToMove = 0; //may change later, asking user

	for (int i = 0; i < _nPlayers;++i) {
		_players.push_back(new Player(*_pool,playerNames.at(i)));
	}
}

void Game::askMove() {
    Player player = *_players.at(_currentToMove);
    player.showHand();
    player.showScore();
    std::string input;
    for (;;) {
        std::cout << "\nMove: ";
        std::getline(std::cin, input);
        Command command(input);
        if (command.getCommand() == 2) {
            if (player.mayMove(*_board, *_pool)) std::cout << "Oops! You have a valid move, so you can not exchange from pool.\n";
            else {
                player.exchange(1, *_pool);
                std::cout << "exchanging from pool...\n";
            }
        }
        else if (command.isMove()) {
            Move move(command, *_board);
            if (!move.hasProblems(player)) {
                move.execute(player, *_board, *_pool);
                break;
            }
            else std::cout << "move has problems.\n";
        }
        else std::cout << "Not recognized or implemented yet.\n";
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