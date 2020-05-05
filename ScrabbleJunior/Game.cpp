#include "Game.h"
#include "../common/ConsoleSetup.h"

Game::Game(Board* board, std::vector<std::string> playerNames, std::vector<int> playerForeColors, int firstToMove) {
	_nPlayers = playerNames.size();
	_board = board;
	_pool = new Pool(board);
	_playerForeColors = playerForeColors;

	for (int i = 0; i < _nPlayers;++i) {
		_players.push_back(new Player(_pool, playerNames.at(i), playerForeColors.at(i)));
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

void Game::askCommand(int turnNumber) {
	if (_currentPlayer->mayPass() && !_currentPlayer->mayMove(_board, _pool)) return;
	_currentPlayer->resetExchangeCount();
	std::cout << "You have on hand: ";
	_currentPlayer->showHand();
	std::string input;
	for (;;) {
		std::cout << "\n(turn " << turnNumber << ") " << _currentPlayer->getName() << ": ";
		std::getline(std::cin, input);
		Command command(input);

		if (command.isMove()) {
			Move move(&command, _board);
			int problemLevel = move.hasProblems(_currentPlayer);
			if (problemLevel) {
				std::cout << "Could not play! ";
				if (problemLevel == 1)
					std::cout << "Are you attempting to put a tile outside the board?\n";
				if (problemLevel == 2)
					std::cout << "Is the letter " << command.getMoveLetter() << " in that position?\n";
				if (problemLevel == 3)
					std::cout << "You do not have the letter " << command.getMoveLetter() << " in hand.\n";
				if (problemLevel == 4)
					std::cout << "That position already has a tile.\n";
				if (problemLevel == 5)
					std::cout << "Are you actually starting or continuing a word?\n";
			}
			else {
				move.execute(_currentPlayer, _board, _pool);
				break;
			}
		}

		else if (command.hasNoConflicts()) {

			if (command.isExchange()) {
				int token = command.getExchangeLetter();
				if (_currentPlayer->mayMove(_board, _pool)) {
					std::cout << "You can not exchange when you have possible moves. Pay attention!\n";
				}
				else if (!_currentPlayer->exchange(token, _pool)) {
					if (!_pool->getCurrentSize()) {
						_currentPlayer->forcePass();
						std::cout << "The pool is empty. You may pass your move.\n";
						std::cout << "Press enter to continue.\n";
						std::cin.get();
						std::cin.ignore(10000, '\n');
					}
					else std::cout << "Could not exchange. Have you got the letter " << command.getExchangeLetter() << " on hand?\n";
				}
				else {
					std::cout << "The exchange was successful!\n";
					std::cout << "You have now on hand: "; _currentPlayer->showHand();
				}
			}

			else if (command.isCheckHands()) showHands();
			else if (command.isCheckScores()) showScores();
			else if (command.isCheckPool()) _pool->show();
			else if (command.isHelp()) std::cout << "***Instructions will be shown here***\n";
			else if (command.isHint()) {
				coord pos = _currentPlayer->getPossiblePos(_board, _pool);
				if (pos.hCollumn == -1 || pos.vLine == -1) std::cout << "Maybe you can't move right now...\n";
				else std::cout << "Look carefully at the board on position " << (char)('A' + pos.vLine) << (char)('a' + pos.hCollumn) << "...\n";
			}
			else if (command.isPass()) {
				if (_currentPlayer->mayMove(_board, _pool)) {
					std::cout << "You cannot pass when you have possible moves. Look carefully!\n";
				}
				else if (_pool->getCurrentSize() && !_currentPlayer->getExchangeCount()) {
					std::cout << "The pool is not empty. Please try to exchange a letter.\n";
				}
				else {
					_currentPlayer->forcePass();
					break;
				}
			}
			else std::cout << "Command not recognized. Please type 'help' to view available commands.\n";

			if (std::cin.fail()) {
				std::cin.ignore(10000, '\n');
				std::cin.clear();
			}
		}
	}
}

void Game::nextTurn() {
	_currentPlayerPos++;
	if (_currentPlayerPos == _nPlayers) _currentPlayerPos = 0;
	_currentPlayer = _players.at(_currentPlayerPos);
	_currentPlayer->doNotPass();
}

bool Game::allPlayersMustPass() const {
	for (Player* player : _players) {
		if (!player->mayPass()) return false;
	}
	return true;
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
	return !allPlayersMustPass();
}

bool Game::hasWinner() const {
	return getWinner() != -1;
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

void Game::showScores() const {
	for (auto player : _players) {
		std::cout << player->getName() << std::endl;
		std::cout << player->getScore() << " points";
		std::cout << std::endl;
	}
}

void Game::showHands() const {
	for (auto player : _players) {
		std::cout << player->getName() << std::endl;
		player->showHand();
		std::cout << std::endl;
	}
}