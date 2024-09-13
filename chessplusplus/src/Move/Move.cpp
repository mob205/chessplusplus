#include "Move/Move.h"
#include "Board/Board.h"
#include "Piece/Piece.h"

MoveResult Move::executeMove(Board& board, std::function<char()> inputCallback)
{
	if (board[end])
	{
		captured = std::move(board[end]);
	}
	board[end] = std::move(board[start]);
	board[end]->updatePosition(end);

	originalMoved = board[end]->getMoved();
	board[end]->setMoved(true);

	return MoveResult{};
}

void Move::undoMove(Board& board)
{
	board[end]->setMoved(originalMoved);

	board[start] = std::move(board[end]);
	board[start]->updatePosition(start);

	board[end] = std::move(captured);
}