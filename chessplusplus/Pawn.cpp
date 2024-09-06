#include "Pawn.h"
#include "Board.h"

MoveSet Pawn::getPossibleMoves(Board& board) const
{
	MoveSet set{};
	Point ahead{ position + forward() };
	return {};
}