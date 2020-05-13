#include "Game.h"
#include "../common/ConsoleSetup.h"

Game::Game(Board* board, std::vector<std::string> &playerNames, std::vector<int> &playerForeColors, int firstToMove) {
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
	if (_board->getDimensions().vLine >= COMPACT_VIEW_MAX) _compactCardView = false;
	else _compactCardView = true;
}

Game::~Game() {
	if (_pool!=NULL) delete _pool;
	for (int i = 0; i < _nPlayers;++i) {
		if (_players.at(i) != NULL) delete _players.at(i);
	}
}

void Game::showBoardAndCardView(const std::string &view, bool showInfo) const {
	clearConsole();
	_board->show();
	if (view == "hands") showHands(showInfo);
	else showScores(showInfo);
}

void Game::askCommand(int turnNumber) {
	bool ableToMove = _currentPlayer->mayMove(_board);
	if (_currentPlayer->getMayPass() && ableToMove) _currentPlayer->doNotPass();
	_currentPlayer->resetExchangeCount();

	std::string commandPrompt, regularMessage, input;
	std::vector<std::string> coloredMessage;
	int playerColor = _currentPlayer->getColor();

	if (_currentPlayer->getMayPass() && !ableToMove) { //passed before and still can't move
		if (turnNumber == 1) {
			coloredMessage = {
				_currentPlayer->getName() + ", you still cannot move.",
				"We will skip your turn automatically.",
			};
		}
		else return;
	}

	if (!_currentPlayer->getActualHandSize() && !coloredMessage.size()) { //player has nothing on hand
		_currentPlayer->forcePass();
		coloredMessage = {
			_currentPlayer->getName() + ", you have nothing on your hand.",
			"Your turn will be skipped as long as you cannot move."
		};
	}

	showBoardAndCardView();

	for (;;) {

		if (!coloredMessage.size()) {

			commandPrompt = "(turn " + std::to_string(turnNumber) + ") " + _currentPlayer->getName() + ": ";
			regularMessage = "", coloredMessage = {};

			paddingAndTopic(playerColor,true);
			std::cout << commandPrompt;

			std::getline(std::cin, input); cleanBuffer();
			Command command(input);

			if (command.isMove()) {
				Move move(&command, _board);
				int problemLevel = move.hasProblems(_currentPlayer);
				if (problemLevel) {
					regularMessage = "Could not play! ";
					if (problemLevel == 1) {
						regularMessage += "Are you attempting to put a tile outside the board?";
					}
					else if (problemLevel == 2) {
						regularMessage += "Is the letter ";
						regularMessage += command.getMoveLetter();
						regularMessage += " in that position?";
					}
					else if (problemLevel == 3) {
						regularMessage += "You do not have the letter ";
						regularMessage += command.getMoveLetter();
						regularMessage += " on hand.";
					}
					else if (problemLevel == 4) {
						regularMessage += "That position already has a tile.";
					}
					else if (problemLevel == 5) {
						regularMessage += "Are you actually starting or continuing a word?";
					}
				}
				else {
					move.execute(_currentPlayer, _board, _pool);
					break;
				}
			}

			else if (command.hasNoConflicts()) {

				if (command.isExchange()) {
					char token = command.getExchangeLetter();
					if (ableToMove) {
						regularMessage = "You can not exchange when you have possible moves. Pay attention!";
					}
					else if (!_pool->getCurrentSize()) {
						_currentPlayer->forcePass();
						coloredMessage = {
						"The pool is empty.",
						"Your turn will be skipped as long as you cannot move."
						};
					}
					else if (_currentPlayer->getExchangeCount()) {
						_currentPlayer->forcePass();
						coloredMessage = {
						"You already exchanged once and still cannot move.",
						"Your turn will be skipped as long as you cannot move."
						};
					}
					else if (!_currentPlayer->exchange(token, _pool)) {
						regularMessage = "Could not exchange. Have you got the letter ";
						regularMessage += command.getExchangeLetter();
						regularMessage += " on hand?";
					}
					else showBoardAndCardView(); //exchange was successful
				}

				else if (command.isCheckHands()) showHands();
				else if (command.isCheckScores()) showScores();
				else if (command.isCheckPool()) showPool();
				else if (command.isHelp()) showHelp();
				else if (command.isHint()) {
					coord pos = _currentPlayer->getPossiblePos(_board);
					if (pos.hCollumn == -1 || pos.vLine == -1) regularMessage = "Maybe you can't move right now...";
					else {
						regularMessage = "Look carefully at the board on position ";
						regularMessage += (char)('A' + pos.vLine);
						regularMessage += (char)('a' + pos.hCollumn);
						regularMessage += "...";
					}
				}
				else if (command.isPass()) {
					if (ableToMove) {
						regularMessage = "You cannot pass when you have possible moves. Look carefully!";
					}
					else if (_pool->getCurrentSize() && !_currentPlayer->getExchangeCount()) {
						regularMessage = "The pool is not empty. Please try to exchange a letter before passing.";
					}
					else {
						_currentPlayer->forcePass();
						break;
					}
				}
				else if (command.isClear()) showBoardAndCardView();
				else regularMessage = smartCommandAdvice(command.getStr());
			}
			else regularMessage = "We found overlapping command keywords in your input. Type 'help' to learn why.";
		}

		if (regularMessage.size()) {
			paddingAndTopic(WHITE, true);
			std::cout << regularMessage << "\n";
		}

		if (coloredMessage.size()) {
			coloredMessage.push_back("Press enter to continue. ");
			std::cout << "\n";
			for (auto i : coloredMessage) {
				paddingAndTopic(playerColor, false);
				std::cout << i << "\n";
			}
			askEnter();
			return;
		}
	}
}

void Game::nextTurn() {
	_currentPlayerPos++;
	if (_currentPlayerPos == _nPlayers) _currentPlayerPos = 0;
	_currentPlayer = _players.at(_currentPlayerPos);
}

bool Game::allPlayersMustPass() const {
	for (Player* player : _players) {
		if (!player->getMayPass()) return false;
	}
	return true;
}

bool Game::hasFinished() const {
	for (auto& player : _players) {
		if (player->mayMove(_board)) return false;
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
		else if (score > maxScore) {
			currentWinner = i;
			maxScore = score;
		}
	}
	return currentWinner;
}

void Game::showScores(bool showTurnInfo) const {
	std::function<void(int, int)> write = [&](int line, int col) {

		for (int i = 0; i < _nPlayers;++i) {
			Player* player = _players.at(i);

			putCursorOnPos(line++, col);
			printForeColor(player->getColor(), '|');
			std::cout << player->getName();
			if (showTurnInfo) {
				if (i == _currentPlayerPos) std::cout << " - to play!";
				else if (player->getMayPass()) std::cout << " - passed last turn";
			}
			std::cout << std::endl;

			putCursorOnPos(line++, col);
			printForeColor(player->getColor(), '|');
			std::cout << player->getScore() << " points";

			for (int j = 0; j < 5 - _nPlayers - _compactCardView; ++j) line++;
		}
	};

	writeCardView(_board->getDimensions().vLine, _board->getDimensions().hCollumn, write);
}

void Game::showHands(bool showTurnInfo) const {
	std::function<void(int,int)> write = [&](int line, int col){
		for (int i = 0; i < _nPlayers;++i) {
			Player* player = _players.at(i);

			putCursorOnPos(line++, col);
			printForeColor(player->getColor(), '|');
			std::cout << player->getName();
			if (showTurnInfo) {
				if (i == _currentPlayerPos) std::cout << " - to play!";
				else if (player->getMayPass()) std::cout << " - passed last turn";
			}
			std::cout << std::endl;

			putCursorOnPos(line++, col);
			printForeColor(player->getColor(), '|');
			if (i == _currentPlayerPos) player->showHand(true);
			else player->showHand(false);

			for (int j = 0; j < 5 - _nPlayers - _compactCardView; ++j) line++;
		}
	};

	writeCardView(_board->getDimensions().vLine, _board->getDimensions().hCollumn, write);
}

void Game::showHelp() const {
	std::function<void(int, int)> write = [&](int line, int col) {
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
				std::cout << sentence << std::endl;
				putCursorOnPos(line++, col);
			}
		}
		for (auto sentence : commands) {
			std::cout << sentence << std::endl;
			putCursorOnPos(line++, col);
		}
	};

	writeCardView(_board->getDimensions().vLine, _board->getDimensions().hCollumn, write);
}

void Game::showPool() const {
	std::function<void(int, int)> write = [&](int line, int col) {

		std::vector<char> letters = _pool->getAllLetters();
		int size = letters.size();

		std::cout << "|" << size << " letters on the pool";
		putCursorOnPos(line++, col);

		for (int i = 0; i < size;++i) {
			if (i % 11 == 0) {
				putCursorOnPos(line++, col);
				std::cout << "|";
			}
			std::cout << letters.at(i) << " ";
		}
	};

	writeCardView(_board->getDimensions().vLine, _board->getDimensions().hCollumn, write);
}

std::string Game::getPlayerName(int playerPos) const {
	if (playerPos >= _nPlayers || playerPos < 0) return "Not found";
	else return _players.at(playerPos)->getName();
}

void Game::end() const {
	showBoardAndCardView("scores", false);

	int color;
	int winner = getWinner();

	if (hasWinner()) color = _players.at(winner)->getColor();
	else color = WHITE;

	paddingAndTopic(WHITE, true); std::cout << "THE GAME HAS ENDED!\n";

	if (allPlayersMustPass()) {
		paddingAndTopic(WHITE, true);
		std::cout << "All players passed their moves.\n";
	}

	paddingAndTopic(color, true);
	if (hasWinner()) std::cout << _players.at(winner)->getName() << " won with brilliancy!\n";
	else std::cout << "There has been a draw! Congratulations to all.\n";

	paddingAndTopic(WHITE, true); std::cout << "Press enter twice to exit.\n";
	int i = 2; while (i--) askEnter();
}