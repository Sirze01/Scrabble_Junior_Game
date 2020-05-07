#pragma once
#include <vector>
#include "Player.h"
#include <string>
#include "../common/Board.h"
#include "Pool.h"
#include "Move.h"
#include "Command.h"

class Game{
public:
	Game(Board* board, std::vector<std::string> playerNames, std::vector<int> playerForeColors, int firstToMove);
	~Game();
	void askCommand(int turnNumber);
	void nextTurn();
	void showScores() const;
	void showHands() const;
	void showHelp() const;
	void showPool() const;
	bool hasFinished() const;
	bool allPlayersMustPass() const;
	bool hasWinner() const;
	int getWinner() const;
	std::string getPlayerName(int playerPos) const;
private:
	int _nPlayers;
	std::vector<int> _playerForeColors;
	int _currentPlayerPos;
	Player* _currentPlayer;
	std::vector<Player*> _players;
	Board* _board;
	Pool* _pool;
	bool _compactCardView;
};

