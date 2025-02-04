#include "Move/StandardMove.h"
#include "Board/Board.h"
#include "Piece/PieceEnums.h"

MoveResult StandardMove::executeMove(Board& board, char extraInput)
{
	MoveResult res = Move::executeMove(board);

	res.moveType = MoveResult::Type::Standard;
	res.standard = MoveResult::StandardResult{};
	return res;
}

void StandardMove::undoMove(Board& board)
{
	Move::undoMove(board);
}