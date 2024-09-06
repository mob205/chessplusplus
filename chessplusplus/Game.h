#pragma once

#include "Board.h"
#include "Piece.h"

class Game
{
public:
	Game();
	Board& getBoard() { return board; }

private:
	Board board{};
};