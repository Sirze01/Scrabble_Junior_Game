#pragma once
#include <vector>
#include "Player.h"
#include <string>
#include "../common/Board.h"
#include "Pool.h"

class Game{
public:
	Game(Board &board, int nPlayers, std::vector<std::string> _playerNames);
	void askMove();
	void nextTurn();
private:
	int _nPlayers;
	int currentToMove;
	std::vector<Player> _players;
	Board _board;
	Pool _pool;
};

