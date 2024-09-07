#include "DoublePawnMove.h"
#include "Board.h"
#include "Pawn.h"

void DoublePawnMove::ExecuteMove(Board& board) const
{
	Move::ExecuteMove(board);

	// Guaranteed to be a pawn, since only pawns should use DoublePawnMove
	static_cast<Pawn*>(board[end].get())->updateDoubleMove();
}