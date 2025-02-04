#include "Move/StandardMove.h"
#include "Board/Board.h"
#include "Piece/PieceEnums.h"

MoveResult StandardMove::executeMove(Board& board, char extraInput)
{
	MoveResult res = Move::executeMove(board);

	res.moveType = MoveResult::Type::Standard;
	res.movedPieceType = board[end]->getType();
	res.standard = MoveResult::StandardResult{ (captured) ? captured->getType() : PieceEnums::None };
	return res;
}

void StandardMove::undoMove(Board& board)
{
	Move::undoMove(board);
}