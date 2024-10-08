#pragma once
#include "Move/Move.h"
#include "Piece/PieceEnums.h"

class PawnMove : public Move
{
public:
	PawnMove(const Point& start, const Point& end, bool isAttacking = false)
		: Move{start, end, isAttacking}
	{}
	MoveResult executeMove(Board& board, char extraInput = '\0') override;
	void undoMove(Board& board) override;

private:
	PieceEnums::Type promotePawn(Board& board, char extraInput);

	// Keep the promoted pawn in case of undo
	std::unique_ptr<Piece> promotedPawn{};

	// Non-owning view to the newly promoted piece on the board
	const Piece* promotedPiece{};
};