#include "Game.h"
#include "../common/ConsoleSetup.h"
#include "../common/StringProcess.h"

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

	//card settings
	if (_board->getDimensions().vLine > 12) _compactCardView = false;
	else _compactCardView = true;
}

Game::~Game() {
	delete _pool;
	for (int i = 0; i < _nPlayers;++i) {
		delete _players.at(i);
	}
}

void Game::askCommand(int turnNumber) {
	if (_currentPlayer->mayPass() && !_currentPlayer->mayMove(_board, _pool)) return;
	if (_currentPlayer->mayPass() && _currentPlayer->mayMove(_board, _pool)) _currentPlayer->doNotPass();

	auto showBoardAndHand = [&]() { //[&] means capture everything outside this scope
		clearConsole();
		_board->show();
		showScores();
		std::cout << "You have on hand: ";
		_currentPlayer->showHand();
	};

	_currentPlayer->resetExchangeCount();
	showBoardAndHand();
	std::string input;

	for (;;) {
		std::cout << "\n(turn " << turnNumber << ") " << _currentPlayer->getName() << ": ";
		std::getline(std::cin, input);

		//make sure input buffer is ok
		if (std::cin.fail()) {
			std::cin.ignore(10000, '\n');
			std::cin.clear();
		}
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
						std::cout << "The pool is empty. Your turn will be skipped as long as you cannot move.\n";
						std::cout << "Press enter to continue.\n";
						std::cin.ignore(10000, '\n');
						break;
					}
					else std::cout << "Could not exchange. Have you got the letter " << command.getExchangeLetter() << " on hand?\n";
				}
				else showBoardAndHand();
			}

			else if (command.isCheckHands()) showHands();
			else if (command.isCheckScores()) showScores();
			else if (command.isCheckPool()) showPool();
			else if (command.isHelp()) showHelp();
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
			else if (command.isClear()) showBoardAndHand();
			else std::cout << smartCommandAdvice(command.getStr());
		}

		else std::cout << smartCommandAdvice(command.getStr());
	}
}

void Game::nextTurn() {
	_currentPlayerPos++;
	if (_currentPlayerPos == _nPlayers) _currentPlayerPos = 0;
	_currentPlayer = _players.at(_currentPlayerPos);
}

bool Game::allPlayersMustPass() const {
	for (Player* player : _players) {
		if (!player->mayPass()) return false;
	}
	return true;
}

bool Game::hasFinished() const {
	if (allPlayersMustPass()) return true;

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
	saveCurrentCursorPosition();

	int line = 2; //initial top padding
	int col = 1 + 2 * (_board->getDimensions().hCollumn) + CARD_LEFT_PADDING;

	eraseCardView(_board->getDimensions().vLine, col);

	for (auto player : _players) {
		for (int i = 0; i < 5 - _nPlayers - _compactCardView; ++i) {
			putCursorOnPos(line++, col);
			std::cout << std::endl;
		}

		putCursorOnPos(line++, col);
		printForeColor(player->getColor(), '|');
		std::cout << player->getName() << std::endl;

		putCursorOnPos(line++, col);
		printForeColor(player->getColor(), '|');
		std::cout << player->getScore() << " points";
	}

	restoreSavedCursorPosition();
}

void Game::showHands() const {
	saveCurrentCursorPosition();

	int line = 2; //initial top padding
	int col = 1 + 2 * (_board->getDimensions().hCollumn) + CARD_LEFT_PADDING;

	eraseCardView(_board->getDimensions().vLine, col);

	for (auto player : _players) {
		for (int i = 0; i < 5 - _nPlayers - _compactCardView; ++i) {
			putCursorOnPos(line++, col);
			std::cout << std::endl;
		}

		putCursorOnPos(line++, col);
		printForeColor(player->getColor(), '|');
		std::cout << player->getName() << std::endl;

		putCursorOnPos(line++, col);
		printForeColor(player->getColor(), '|');
		player->showHand();
	}

	restoreSavedCursorPosition();
}

void Game::showHelp() const {
	saveCurrentCursorPosition();

	int line = 3; //initial top padding
	int col = 1 + 2 * (_board->getDimensions().hCollumn) + CARD_LEFT_PADDING;

	eraseCardView(_board->getDimensions().vLine, col);

	std::vector<std::string> intro =
	{
		"|Start or continue words with the tiles you have on hand.",
		"|You get one point for each word you complete.",
		"|Available commands:",
		"|"
	};

	std::vector<std::string> commands =
	{
		"|-> 'move <Yx> <letter>' - play letter on position.",
		"|-> 'exchange <letter>' - exchange a letter from the pool.",
		"|-> 'check hands' - have a look at all players' hands.",
		"|-> 'check scores' - have a look at the current scores.",
		"|-> 'check pool' - spy on the current state of the pool.",
		"|-> 'get hint' - get some advice. Do not abuse of this!",
		"|-> 'pass' - skip turn when you have no possible moves.",
		"|-> 'clear' - erase command history and reload screen."
	};

	if (!_compactCardView) {
		for (auto sentence : intro) {
			putCursorOnPos(line++, col);
			std::cout << sentence << std::endl;
		}
	}
	for (auto sentence : commands) {
		putCursorOnPos(line++, col);
		std::cout << sentence << std::endl;
	}

	restoreSavedCursorPosition();
}

void Game::showPool() const {
	saveCurrentCursorPosition();

	int line = 3; //initial top padding
	int col = 1 + 2 * (_board->getDimensions().hCollumn) + CARD_LEFT_PADDING;

	std::vector<char> letters = _pool->getAllLetters();
	int size = letters.size();

	eraseCardView(_board->getDimensions().vLine, col);
	putCursorOnPos(line++, col);
	std::cout << "|" << size << " letters on the pool";
	putCursorOnPos(line++, col);


	for (int i = 0; i < size;++i) {
		if (i % 11 == 0) {
			std::cout << std::endl;
			putCursorOnPos(line++, col);
			std::cout << "|";
		}
		std::cout << letters.at(i) << " ";
	}

	restoreSavedCursorPosition();
}