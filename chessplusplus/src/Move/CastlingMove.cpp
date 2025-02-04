#include "Move/CastlingMove.h"
#include "Board/Board.h"
#include "Piece/Piece.h"
#include "Move/MoveResult.h"

MoveResult CastlingMove::executeMove(Board& board, char extraInput)
{
	MoveResult res = Move::executeMove(board);
	board[end] = std::move(board[start]);
	board[rookEnd] = std::move(board[rookStart]);

	board[end]->setMoved(true);
	board[rookEnd]->setMoved(true);

	board[end]->setPosition(end);
	board[rookEnd]->setPosition(rookEnd);

	res.moveType = MoveResult::Type::Castle;
	res.movedPieceType = board[end]->getType();
	res.castle = MoveResult::CastleResult{ rookStart, rookEnd };
	return res;
}

void CastlingMove::undoMove(Board& board)
{
	board[start] = std::move(board[end]);
	board[rookStart] = std::move(board[rookEnd]);

	board[start]->setMoved(false);
	board[rookStart]->setMoved(false);

	board[start]->setPosition(start);
	board[rookStart]->setPosition(rookStart);
}
