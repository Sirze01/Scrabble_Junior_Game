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
	Game(Board& board, const std::vector<std::string>& playerNames,
		const std::vector<int>& playerForeColors, const std::vector<bool>& botFlags, int firstToMove);
    void showEndMessage() const;
	void askCommand(int turnNumber);
	void nextTurn();
	bool hasFinished() const;
	bool hasWinner() const;
	int getWinner() const;
	std::string getPlayerName(int playerPos) const;
private:
    bool allHighlighted() const;
    bool allPlayersMustPass() const;
    void showPlayerInfo(const std::string& info, bool showTurnInfo = true) const;
    void showHelp() const;
    void showPool() const;
    void showBoardAndCardView(const std::string &view = "hands", bool showTurnInfo = true) const;

    int _nPlayers;
	std::vector<int> _playerForeColors;
	int _currentPlayerPos;
	Player* _currentPlayer;
	std::vector<Player> _players;
	Board& _board;
	Pool _pool;
    bool _compactCardView;
};
