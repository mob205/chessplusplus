#include "Move.h"
#include "Board.h"
#include "Piece.h"

void Move::ExecuteMove(Board& board) const
{
	if (board[end])
	{
		std::cout << "Captured an enemy " << board[end]->getName() << ".\n";
	}
	board[end] = std::move(board[start]);
	board[end]->updatePosition(end);
	board[end]->setMoved(true);
}