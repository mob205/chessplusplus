#include "Move/CastlingMove.h"
#include "Board/Board.h"
#include "Piece/Piece.h"

void CastlingMove::executeMove(Board& board, std::function<char()> callback)
{
	board[end] = std::move(board[start]);
	board[rookEnd] = std::move(board[rookStart]);

	board[end]->setMoved(true);
	board[rookEnd]->setMoved(true);

	board[end]->updatePosition(end);
	board[rookEnd]->updatePosition(rookEnd);
}

void CastlingMove::undoMove(Board& board)
{
	board[start] = std::move(board[end]);
	board[rookStart] = std::move(board[rookEnd]);

	board[start]->setMoved(false);
	board[rookStart]->setMoved(false);

	board[start]->updatePosition(start);
	board[rookStart]->updatePosition(rookStart);
}

void CastlingMove::printMove() const
{
	std::cout << "Castled!\n";
}
