#include "Move/StandardMove.h"
#include "Board/Board.h"
#include "Piece/PieceEnums.h"

MoveResult StandardMove::executeMove(Board& board, std::function<char()> callback)
{
	Move::executeMove(board);

	MoveResult res{};
	res.type = MoveResult::Type::Standard;
	res.standard = MoveResult::StandardResult{ start, end, (captured) ? captured->getType() : PieceEnums::None };
	return res;
}

void StandardMove::undoMove(Board& board)
{
	Move::undoMove(board);
}