#include "Game.h"

Game::Game(Board& board, int nPlayers, std::vector<std::string> _playerNames) {
	_nPlayers = nPlayers;
	_players.reserve(nPlayers);
	_pool = *(new Pool(board));

	for (int i = 0; i < nPlayers;++i) {
		_players.at(i) = *(new Player(_pool,_playerNames.at(i)));
	}
}