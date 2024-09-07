#pragma once

#include "Board.h"
#include "Piece.h"

class Game
{
	friend void playGame(Game& game);

public:
	Game();

	Board& getBoard() { return board; }

private:
	Board board{};
	int currentTurn{};
	Piece::Team currentTeam{Piece::White};
};

