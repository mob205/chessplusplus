#include "StandardMove.h"
#include "Board.h"
#include "BoardHelpers.h"
#include "Piece.h"

StandardMove::StandardMove(Point start, Point end)
	: start{ start }, end{ end }
{}

void StandardMove::ExecuteMove(Board& board) const
{
	board[end] = std::move(board[start]);
	board[end]->UpdatePosition(end);
}