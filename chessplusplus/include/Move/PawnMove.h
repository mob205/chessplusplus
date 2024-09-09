#pragma once
#include "Move/Move.h"
#include "Piece/Piece.h"

class PawnMove : public Move
{
public:
	PawnMove(const Point& start, const Point& end, bool isAttacking = false)
		: Move{start, end, isAttacking}
	{}
	void executeMove(Board& board, std::function<char()> callback) override;
	void undoMove(Board& board) override;
	void printMove() const override;

private:
	void promotePawn(Board& board, std::function<char()> callback);

	// Keep the promoted pawn in case of undo
	std::unique_ptr<Piece> promotedPawn{};

	// Non-owning view to the newly promoted piece on the board
	const Piece* promotedPiece{};
};