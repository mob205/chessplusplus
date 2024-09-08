#include "EnPassantMove.h"
#include "Board.h"
#include "Piece.h"


void EnPassantMove::executeMove(Board& board)
{
	// Move the pawn
	Move::executeMove(board);

	// Capture piece
	// Captured should be nullptr after the base Move since it's impossible to en passant and diagonal capture at the same time
	captured = std::move(board[captureSpot]);
}

void EnPassantMove::undoMove(Board& board)
{
	board[end]->setMoved(originalMoved);

	board[start] = std::move(board[end]);
	board[start]->updatePosition(start);

	board[captureSpot] = std::move(captured);
}