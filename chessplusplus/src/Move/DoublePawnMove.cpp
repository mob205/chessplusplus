#include "Move/DoublePawnMove.h"
#include "Board/Board.h"
#include "Piece/Pawn.h"

MoveResult DoublePawnMove::executeMove(Board& board, char extraInput)
{
	MoveResult res = Move::executeMove(board);

	// Guaranteed to be a pawn, since only pawns should use DoublePawnMove
	static_cast<Pawn*>(board[end].get())->setDoubleMove();

	res.moveType = MoveResult::Type::DoublePawn;
	res.movedPieceType = board[end]->getType();
	res.doublePawn = MoveResult::DoublePawnResult{};
	return res;
}

void DoublePawnMove::undoMove(Board& board)
{
	Move::undoMove(board);

	static_cast<Pawn*>(board[start].get())->unsetDoubleMove();
}