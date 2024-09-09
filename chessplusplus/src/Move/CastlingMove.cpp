#include "Move/CastlingMove.h"
#include "Board/Board.h"
#include "Piece/Piece.h"

void CastlingMove::executeMove(Board& board)
{
	board[end] = std::move(board[start]);
	board[rookEnd] = std::move(board[rookStart]);

	board[end]->setMoved(true);
	board[rookEnd]->setMoved(true);
}

void CastlingMove::undoMove(Board& board)
{
	board[start] = std::move(board[end]);
	board[rookStart] = std::move(board[rookEnd]);

	board[start]->setMoved(false);
	board[rookStart]->setMoved(false);
}

void CastlingMove::printMove() const
{
	std::cout << "Castled!\n";
}
