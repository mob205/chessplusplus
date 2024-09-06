#pragma once

#include "Board.h"
#include "Piece.h"

class Game
{
public:
	Board& getBoard() { return board; }

private:
	Board board{};
};