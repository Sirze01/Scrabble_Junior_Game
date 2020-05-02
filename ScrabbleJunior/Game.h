#pragma once
#include <vector>
#include "Player.h"
#include <string>
#include "../common/Board.h"
#include "Pool.h"
#include "Move.h"

class Game{
public:
	Game(Board* board, std::vector<std::string> _playerNames);
	~Game();
	void askMove();
	void nextTurn();
	bool hasFinished() const;
private:
	int _nPlayers;
	int _currentToMove;
	std::vector<Player*> _players;
	Board* _board;
	Pool* _pool;
};

