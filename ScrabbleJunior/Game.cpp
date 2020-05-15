#include "Game.h"
#include "../common/ConsoleSetup.h"
#include "../common/StringProcess.h"
#include <windows.h>

Game::Game(Board& board, const std::vector<std::string>& playerNames,
	const std::vector<int>& playerForeColors, const std::vector<bool>& botFlags, int firstToMove) :
	_pool{board}, _board{board}, _playerForeColors{ playerForeColors },
	_nPlayers{ static_cast<int>(playerNames.size()) }, _currentPlayerPos{ firstToMove }
{

	for (int i = 0; i < _nPlayers;++i) {
		Player player = Player(_pool, playerNames.at(i), playerForeColors.at(i), botFlags.at(i));
		_players.push_back(player);
	}

	_currentPlayer = &_players.at(_currentPlayerPos);

	//card settings
	if (_board.getDimensions().vLine >= COMPACT_VIEW_MAX) _compactCardView = false;
	else _compactCardView = true;
}

void Game::showBoardAndCardView(const std::string& view, bool showTurnInfo) const {
	clearConsole();
	_board.show();
	showPlayerInfo(view, showTurnInfo);
}

void Game::askCommand(int turnNumber) {

	_currentPlayer->resetExchangeCount();
	bool ableToMove = _currentPlayer->mayMove(_board);

	if (_currentPlayer->getHasPassed()) {
		if (ableToMove) {
			_currentPlayer->doNotPass();
		}
		else if (turnNumber == 2) {
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
				const coord pos = _currentPlayer->getPossibleMovePos(_board);
				char letter = _board.getLetters().at(pos.vLine).at(pos.hColumn);
				const Move move(pos, letter, _board);
				move.execute(*_currentPlayer, _board, _pool);
				coloredMessage << name << " will play " << _board.getPositionString(pos) << SPACE << letter << ".\n";
				break;
			}
			else if (!_currentPlayer->getExchangeCount() && _currentPlayer->getHandSize() && _pool.getCurrentSize()) {
				int handPos = randomBetween(0, _currentPlayer->getHandSize() - 1);
				char oldLetter = _currentPlayer->getHand().at(handPos);
				char newLetter = _currentPlayer->exchange(oldLetter, _pool);
				coloredMessage << name << " exchanged letter " << oldLetter << " from the pool and got letter " << newLetter << ".\n\n";
				ableToMove = _currentPlayer->mayMove(_board);
				showPlayerInfo("hands");
			}
			else {
				coloredMessage << name << " cannot move and thus will pass.\n";
				_currentPlayer->forcePass();
				break;
			}
		}
	}

	else {
		if (!ableToMove) {
			if (_currentPlayer->getHasPassed()) {
				coloredMessage << name << ", you still cannot move.\n" <<
					"We will skip your turn automatically.\n";
			}
			else if (!_currentPlayer->getHandSize()) {
				_currentPlayer->forcePass();
				coloredMessage << name << ", you have nothing on your hand.\n" <<
					"Your turn will be skipped as long as you cannot move.\n";
			}
		}
	}

	std::string commandPrompt, input;

	for (;;) {
		std::stringstream regularMessage;

		if (isEmpty(coloredMessage)) {
			commandPrompt = "(turn " + std::to_string(turnNumber) + ") " + name + ": ";

			paddingAndTopic(playerColor, true);
			std::cout << commandPrompt;

			std::getline(std::cin, input); cleanBuffer();
			const Command command(input);

			if (command.isMove()) {
				const Move move(command, _board);
				int problemLevel = move.hasProblems(*_currentPlayer);
				if (problemLevel) {
					regularMessage << "Could not play! ";
					if (problemLevel == 1) {
						regularMessage << "You are attempting to put a tile outside the board.\n";
					}
					else if (problemLevel == 2) {
						regularMessage << "The letter " << command.getMoveLetter() << " is not in that position.\n";
					}
					else if (problemLevel == 3) {
						regularMessage << "You do not have the letter " << command.getMoveLetter() << " on hand.\n";
					}
					else if (problemLevel == 4) {
						regularMessage << "That position already has a tile.\n";
					}
					else if (problemLevel == 5) {
						regularMessage << "You are not starting or continuing a word.\n";
					}
				}
				else {
					move.execute(*_currentPlayer, _board, _pool);
					return;
				}
			}

			else if (command.hasNoConflicts()) {

				if (command.isExchange()) {
					char token = command.getExchangeLetter();
					if (ableToMove) {
						regularMessage << "You can not exchange when you have possible moves. Pay attention!\n";
					}
					else if (!_currentPlayer->hasOnHand(token)) {
						regularMessage << "Could not exchange! You do not have the letter " << token << " on hand.\n";
					}
					else if (!_pool.getCurrentSize()) {
						_currentPlayer->forcePass();
						coloredMessage << "The pool is empty.\n" <<
							"Your turn will be skipped as long as you cannot move.\n";
					}
					else if (_currentPlayer->getExchangeCount()) {
						_currentPlayer->forcePass();
						coloredMessage << "You already exchanged once and still cannot move.\n" <<
							"Your turn will be skipped as long as you cannot move.\n";
					}
					else {
						char newLetter = _currentPlayer->exchange(token, _pool);
						showPlayerInfo("hands");
						regularMessage << "The exchange was successful. You got letter " << newLetter << " from the pool.\n";
						ableToMove = _currentPlayer->mayMove(_board);
					}
				}

				else if (command.isCheckHands()) showPlayerInfo("hands");
				else if (command.isCheckScores()) showPlayerInfo("scores");
				else if (command.isCheckPool()) showPool();
				else if (command.isHelp()) showHelp();
				else if (command.isHint()) {
					if (!ableToMove) {
						regularMessage << "Maybe you can't move right now...\n";
					}
					else {
						const coord pos = _currentPlayer->getPossibleMovePos(_board);
						regularMessage << "Look carefully at the board on position " << _board.getPositionString(pos) << "...\n";
					}
				}
				else if (command.isPass()) {
					if (ableToMove) {
						regularMessage << "You cannot pass when you have possible moves. Look carefully!\n";
					}
					else if (_pool.getCurrentSize() && !_currentPlayer->getExchangeCount()) {
						regularMessage << "The pool is not empty. Try to exchange a letter before passing.\n";
					}
					else { //allow passing
						_currentPlayer->forcePass();
						return;
					}
				}
				else if (command.isClear()) showBoardAndCardView();
				else regularMessage << smartCommandAdvice(command.getStr());
			}
			else regularMessage << "We found overlapping command keywords in your input. Type 'help' to learn why.\n";
		}

		if (!isEmpty(regularMessage)) {
			paddingAndTopic(WHITE, true);
			std::cout << regularMessage.str();
		}

		else if (!isEmpty(coloredMessage)) {
			coloredMessage << "Press enter to continue.\n";
			std::string sentence;
			while (std::getline(coloredMessage, sentence)) {
				if (sentence.size()) {
					paddingAndTopic(playerColor, true);
				}
				else std::cout << "\n";
				std::cout << sentence;
			}
			std::cout << "\n";
			askEnter();
			return;
		}
	}
}

void Game::nextTurn() {
	_currentPlayerPos++;
	if (_currentPlayerPos == _nPlayers) _currentPlayerPos = 0;
	_currentPlayer = &_players.at(_currentPlayerPos);
}

bool Game::allPlayersMustPass() const {
	for (const Player& player : _players) {
		if (!player.getHasPassed()) return false;
	}
	return true;
}

bool Game::hasFinished() const {
	if (allHighlighted()) return true;
	for (const auto& player : _players) {
		if (!player.getHasPassed() || player.mayMove(_board)) return false;
	}
	return true;
}

bool Game::hasWinner() const {
	return getWinner() != -1;
}

int Game::getWinner() const {
	int maxScore = 0; int currentWinner = -1;
	for (int i = 0; i < _nPlayers;++i) {
		int score = _players.at(i).getScore();
		if (score == maxScore) currentWinner = -1;
		else if (score > maxScore) {
			currentWinner = i;
			maxScore = score;
		}
	}
	return currentWinner;
}

void Game::showPlayerInfo(const std::string& info, bool showTurnInfo) const {
	std::stringstream toWrite;

	for (int i = 0; i < _nPlayers;++i) {
		const Player player = _players.at(i);

		outputForeColor(toWrite, player.getColor(), TOPIC);
		toWrite << player.getName();

		if (showTurnInfo) {
			if (i == _currentPlayerPos) toWrite << " - to play!";
			else if (player.getHasPassed()) toWrite << " - passed last turn";
		}
		toWrite << "\n";

		outputForeColor(toWrite, player.getColor(), TOPIC);

		if (info == "hands") {
			if (i == _currentPlayerPos) player.showHand(toWrite, true);
			else player.showHand(toWrite, false);
		}
		else if (info == "scores") {
			toWrite << player.getScore() << " points\n";
		}
		else {
			toWrite << "No info requested\n";
		}

		for (int j = 0; j < 5 - _nPlayers - _compactCardView; ++j) toWrite << "\n";
	}

	writeCardView(_board.getDimensions().vLine, _board.getDimensions().hColumn, toWrite);
}

void Game::showHelp() const {
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
		for (const auto& sentence : intro) {
			toWrite << sentence << TOPIC << "\n";
		}
	}
	for (const auto& sentence : commands) {
		toWrite << TOPIC << ARROW << SPACE << sentence << "\n";
	}

	writeCardView(_board.getDimensions().vLine, _board.getDimensions().hColumn, toWrite);
}

void Game::showPool() const {
	std::stringstream toWrite;

	std::vector<char> letters = _pool.getAllLetters();
	int size = letters.size();

	toWrite << TOPIC << size << " letters on the pool";

	const int MAX_LETTERS_PER_LINE = 10;
	for (int i = 0; i < size;++i) {
		if (i % (MAX_LETTERS_PER_LINE+1) == 0) {
			toWrite << "\n" << TOPIC;
		}
		toWrite << letters.at(i) << " ";
	}

	writeCardView(_board.getDimensions().vLine, _board.getDimensions().hColumn, toWrite);
}

std::string Game::getPlayerName(int playerPos) const {
	if (playerPos >= _nPlayers || playerPos < 0) return "Not found";
	else return _players.at(playerPos).getName();
}

void Game::showEndMessage() const {
	showBoardAndCardView("scores", false);

	int color = WHITE;
	int winner = getWinner();

	if (hasWinner()) color = _players.at(winner).getColor();

	paddingAndTopic(WHITE, true); std::cout << "THE GAME HAS ENDED!\n";

	if (allPlayersMustPass()) {
		paddingAndTopic(WHITE, true);
		std::cout << "All players passed their moves.\n";
	}

	paddingAndTopic(color, true);
	if (hasWinner()) std::cout << _players.at(winner).getName() << " won with brilliancy!\n";
	else std::cout << "There has been a draw! Congratulations to all.\n";

	paddingAndTopic(WHITE, true); std::cout << "Press enter twice to exit.\n";
	int i = 2; while (i--) askEnter();
}

bool Game::allHighlighted() const {
	coord boardDim = _board.getDimensions();
	std::vector<std::vector<char>> boardLetters = _board.getLetters();
	std::vector<std::vector<bool>> boardHighlights = _board.getHighlights();

	for (size_t line = 0; line < boardDim.vLine; ++line) {
		for (size_t col = 0; col < boardDim.hColumn; ++col) {
			char letter = boardLetters.at(line).at(col);
			if (letter == SPACE) continue;
			if (!boardHighlights.at(line).at(col)) return false;
		}
	}
	return true;
}