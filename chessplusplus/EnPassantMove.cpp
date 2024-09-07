#include "EnPassantMove.h"
#include "Board.h"
#include "Piece.h"


void EnPassantMove::ExecuteMove(Board& board) const
{
	// Move the pawn
	Move::ExecuteMove(board);

	std::cout << "EN PASSANT!!!\n";

	// Capture piece
	board[captureSpot].reset();
}