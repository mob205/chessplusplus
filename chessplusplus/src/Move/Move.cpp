#include "Move/Move.h"
#include "Board/Board.h"
#include "Piece/Piece.h"

MoveResult Move::executeMove(Board& board, char extraInput)
{
	MoveResult res{};
	if (board[end])
	{
		captured = std::move(board[end]);
		res.capturedPieceType = captured->getType();
	}
	board[end] = std::move(board[start]);
	board[end]->setPosition(end);

	originalMoved = board[end]->getMoved();
	board[end]->setMoved(true);

	res.movedPieceType = board[end]->getType();
	res.start = start;
	res.end = end;
	return res;
}

void Move::undoMove(Board& board)
{
	board[end]->setMoved(originalMoved);

	board[start] = std::move(board[end]);
	board[start]->setPosition(start);

	board[end] = std::move(captured);
}