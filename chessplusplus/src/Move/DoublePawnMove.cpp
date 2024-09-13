#include "Move/DoublePawnMove.h"
#include "Board/Board.h"
#include "Piece/Pawn.h"

MoveResult DoublePawnMove::executeMove(Board& board, std::function<char()> callback)
{
	Move::executeMove(board);

	// Guaranteed to be a pawn, since only pawns should use DoublePawnMove
	static_cast<Pawn*>(board[end].get())->setDoubleMove();

	MoveResult res{};
	res.type = MoveResult::Type::DoublePawn;
	res.doublePawn = MoveResult::DoublePawnResult{ start, end };
	return res;
}

void DoublePawnMove::undoMove(Board& board)
{
	Move::undoMove(board);

	static_cast<Pawn*>(board[start].get())->unsetDoubleMove();
}