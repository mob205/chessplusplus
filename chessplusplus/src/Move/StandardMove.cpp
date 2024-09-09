#include "Move/StandardMove.h"
#include "Board/Board.h"
#include "Piece/Piece.h"

void StandardMove::executeMove(Board& board, std::function<char()> callback)
{
	Move::executeMove(board);
}

void StandardMove::undoMove(Board& board)
{
	Move::undoMove(board);
}

void StandardMove::printMove() const
{
	if (captured)
	{
		std::cout << "Captured a " << captured->getName() << '\n';
	}
}