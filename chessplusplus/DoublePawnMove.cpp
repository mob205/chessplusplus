#include "DoublePawnMove.h"
#include "Board.h"
#include "Pawn.h"

void DoublePawnMove::executeMove(Board& board)
{
	Move::executeMove(board);

	// Guaranteed to be a pawn, since only pawns should use DoublePawnMove
	static_cast<Pawn*>(board[end].get())->setDoubleMove();
}

void DoublePawnMove::undoMove(Board& board)
{
	Move::undoMove(board);

	static_cast<Pawn*>(board[start].get())->unsetDoubleMove();
}