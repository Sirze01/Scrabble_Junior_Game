#include "Game.h"
#include "../common/ConsoleSetup.h"

Game::Game(Board* board, const std::vector<std::string> &playerNames,
	const std::vector<int> &playerForeColors, const std::vector<bool> botFlags, int firstToMove) {

	_nPlayers = playerNames.size();
	_board = board;
	_pool = new Pool(board);
	_playerForeColors = playerForeColors;

	for (int i = 0; i < _nPlayers;++i) {
		Player* player = new Player(_pool, playerNames.at(i), playerForeColors.at(i), botFlags.at(i));
		_players.push_back(player);
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

void Game::showBoardAndCardView(const std::string &view, bool showTurnInfo) const {
	clearConsole();
	_board->show();
	showPlayerInfo(view, showTurnInfo);
}

void Game::askCommand(int turnNumber) {

	std::vector<std::string> coloredMessage;
	int playerColor = _currentPlayer->getColor();
	std::string name = _currentPlayer->getName();

	if (_currentPlayer->isBot()) {

		if (!(_currentPlayer->getHasPassed() && turnNumber == 2) && _currentPlayer->getHandSize() && _pool->getCurrentSize() && !_currentPlayer->mayMove(_board)) {
			int handPos = randomBetween(0, _currentPlayer->getHandSize() - 1);
			char letter = _currentPlayer->getHand().at(handPos);
			_currentPlayer->exchange(letter, _pool); //try to exchange one letter silently
			char newLetter = _currentPlayer->getHand().at(handPos);
			coloredMessage.push_back(name + " exchanged letter " + std::string(1,letter) + " from the pool and got letter " + std::string(1,newLetter) + ".");
			coloredMessage.push_back(std::string(1,SPACE));
		}

		if (_currentPlayer->mayMove(_board)) {
			showBoardAndCardView();
			if (_currentPlayer->getHasPassed()) _currentPlayer->doNotPass();
			const coord pos = _currentPlayer->getPossibleMovePos(_board);
			char letter = _board->getLetters().at(pos.vLine).at(pos.hColumn);
			const Move move(pos, letter, _board);
			move.execute(_currentPlayer,_board,_pool);
			coloredMessage.push_back(name + " will play " + _board->getPositionString(pos) + std::string(1,SPACE) + std::string(1,letter) + ".");
		}
		else {
			_currentPlayer->forcePass();
			if (turnNumber == 1) {
				showBoardAndCardView();
				coloredMessage.push_back(name + " cannot move and thus will pass his move.");
			}
			else return;
		}
	}

	_currentPlayer->resetExchangeCount();

	if (_currentPlayer->getHasPassed() && !_currentPlayer->isBot()) {
		bool ableToMove = _currentPlayer->mayMove(_board);
		if (ableToMove) {
			_currentPlayer->doNotPass();
		}
		else {
			if (turnNumber == 1 && !coloredMessage.size()) {
				coloredMessage = {
					name + ", you still cannot move.",
					"We will skip your turn automatically.",
				};
			}
			else return;
		}
	}

	if (!_currentPlayer->getHandSize() && !coloredMessage.size()) { //player has nothing on hand
		_currentPlayer->forcePass();
		coloredMessage = {
			name + ", you have nothing on your hand.",
			"Your turn will be skipped as long as you cannot move."
		};
	}

	if (!_currentPlayer->isBot()) showBoardAndCardView();
	std::string commandPrompt, regularMessage, input;

	for (;;) {

		if (!coloredMessage.size() && !_currentPlayer->isBot()) {

			commandPrompt = "(turn " + std::to_string(turnNumber) + ") " + name + ": ";
			regularMessage = "", coloredMessage = {};

			paddingAndTopic(playerColor,true);
			std::cout << commandPrompt;

			std::getline(std::cin, input); cleanBuffer();
			const Command command(input);

			if (command.isMove()) {
				const Move move(&command, _board);
				int problemLevel = move.hasProblems(_currentPlayer);
				if (problemLevel) {
					regularMessage = "Could not play! ";
					if (problemLevel == 1) {
						regularMessage += "You are attempting to put a tile outside the board.";
					}
					else if (problemLevel == 2) {
						regularMessage += "The letter ";
						regularMessage += command.getMoveLetter();
						regularMessage += " is not in that position.";
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
						regularMessage += "You are not starting or continuing a word.";
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
					if (_currentPlayer->mayMove(_board)) {
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

				else if (command.isCheckHands()) showPlayerInfo("hands");
				else if (command.isCheckScores()) showPlayerInfo("scores");
				else if (command.isCheckPool()) showPool();
				else if (command.isHelp()) showHelp();
				else if (command.isHint()) {
					coord pos = _currentPlayer->getPossibleMovePos(_board);
					if (pos.hColumn == IMPOSSIBLE_MOVE_COORD || pos.vLine == IMPOSSIBLE_MOVE_COORD) {
						regularMessage = "Maybe you can't move right now...";
					}
					else {
						regularMessage = "Look carefully at the board on position " + _board->getPositionString(pos) + "...";
					}
				}
				else if (command.isPass()) {
					if (_currentPlayer->mayMove(_board)) {
						regularMessage = "You cannot pass when you have possible moves. Look carefully!";
					}
					else if (_pool->getCurrentSize() && !_currentPlayer->getExchangeCount()) {
						regularMessage = "The pool is not empty. Try to exchange a letter before passing.";
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
			for (const auto &i : coloredMessage) {
				if (i == std::string(1, SPACE)) {
					std::cout << "\n";
					continue;
				}
				else {
					paddingAndTopic(playerColor, false);
					std::cout << i << "\n";
				}
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
	for (const Player* player : _players) {
		if (!player->getHasPassed()) return false;
	}
	return true;
}

bool Game::hasFinished() const {
	if (allHighlighted()) return true;
	for (const auto& player : _players) {
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

void Game::showPlayerInfo(const std::string &info, bool showTurnInfo) const {
	std::stringstream toWrite;

	for (int i = 0; i < _nPlayers;++i) {
		const Player* player = _players.at(i);

		outputForeColor(toWrite, player->getColor(), '|');
		toWrite << player->getName();

		if (showTurnInfo) {
			if (i == _currentPlayerPos) toWrite << " - to play!";
			else if (player->getHasPassed()) toWrite << " - passed last turn";
		}
		toWrite << "\n";

		outputForeColor(toWrite, player->getColor(), '|');

		if (info == "hands") {
			if (i == _currentPlayerPos) player->showHand(toWrite, true);
			else player->showHand(toWrite, false);
		}
		else if (info == "scores"){
			toWrite << player->getScore() << " points\n";
		}
		else {
			toWrite << "No info requested\n";
		}

		for (int j = 0; j < 5 - _nPlayers - _compactCardView; ++j) toWrite << "\n";
	}

	writeCardView(_board->getDimensions().vLine, _board->getDimensions().hColumn, toWrite);
}

void Game::showHelp() const {
	std::stringstream toWrite;
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
			for (const auto &sentence : intro) {
				toWrite << sentence << "\n";
			}
		}
		for (const auto &sentence : commands) {
			toWrite << sentence << "\n";
		}

	writeCardView(_board->getDimensions().vLine, _board->getDimensions().hColumn, toWrite);
}

void Game::showPool() const {
	std::stringstream toWrite;

		std::vector<char> letters = _pool->getAllLetters();
		int size = letters.size();

		toWrite << "|" << size << " letters on the pool";

		for (int i = 0; i < size;++i) {
			if (i % 11 == 0) {
				toWrite << "\n" << "|";
			}
			toWrite << letters.at(i) << " ";
		}

	writeCardView(_board->getDimensions().vLine, _board->getDimensions().hColumn, toWrite);
}

std::string Game::getPlayerName(int playerPos) const {
	if (playerPos >= _nPlayers || playerPos < 0) return "Not found";
	else return _players.at(playerPos)->getName();
}

void Game::showEndMessage() const {
	showBoardAndCardView("scores", false);

	int color = WHITE;
	int winner = getWinner();

	if (hasWinner()) color = _players.at(winner)->getColor();

	paddingAndTopic(WHITE, true); std::cout << "THE GAME HAS ENDED!\n";

	if (allPlayersMustPass()){
        paddingAndTopic(WHITE, true);
        std::cout << "All players passed their moves.\n";
	}

	paddingAndTopic(color, true);
	if (hasWinner()) std::cout << _players.at(winner)->getName() << " won with brilliancy!\n";
	else std::cout << "There has been a draw! Congratulations to all.\n";

	paddingAndTopic(WHITE, true); std::cout << "Press enter twice to exit.\n";
	int i = 2; while (i--) askEnter();
}

bool Game::allHighlighted() const {
	coord boardDim = _board->getDimensions();
	std::vector<std::vector<char>> boardLetters = _board->getLetters();
	std::vector<std::vector<bool>> boardHighlights = _board->getHighlights();

	for (size_t line = 0; line < boardDim.vLine; ++line) {
		for (size_t col = 0; col < boardDim.hColumn; ++col) {
			char letter = boardLetters.at(line).at(col);
			if (letter == SPACE) continue;
			if (!boardHighlights.at(line).at(col)) return false;
		}
	}
	return true;
}