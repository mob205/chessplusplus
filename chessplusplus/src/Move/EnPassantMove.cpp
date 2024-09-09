#include "Move/EnPassantMove.h"
#include "Board/Board.h"
#include "Piece/Piece.h"


void EnPassantMove::executeMove(Board& board, std::function<char()> callback)
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