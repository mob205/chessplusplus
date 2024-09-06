#include "Pawn.h"
#include "Board.h"

std::vector<IMove> Pawn::getPossibleMoves(Board& board) const
{
	Point ahead{ position + forward() };
	return {};
}