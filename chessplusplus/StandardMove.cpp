#include "StandardMove.h"
#include "Board.h"
#include "Piece.h"

void StandardMove::executeMove(Board& board)
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