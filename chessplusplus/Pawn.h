#pragma once

#include "Piece.h"

class Pawn : public Piece
{
public:
	Pawn(Point position, Team team, const int& currentTurn)
		: Piece{ position, team }, currentTurn{ currentTurn }
	{}

	// Updates the last time a pawn does a double move
	void updateDoubleMove() { turnDoubleMoved = currentTurn; }

	// Returns true if this pawn is eligible to be En Passant'd this turn
	bool isPassantable() const { return turnDoubleMoved != 0 && currentTurn == turnDoubleMoved + 1; }

	// Returns the forward direction for this pawn
	constexpr Point forward() const { return { 1 - (2 * team), 0 }; }

	MoveSet getPossibleMoves(Board& board) const override;

	char getSymbol() const override { return team ? toupper('p') : 'p'; }

	std::string_view getName() const override { return "Pawn"; }

private:
	int turnDoubleMoved{};
	const int& currentTurn{};
};