#pragma once

#include "Piece.h"

class Pawn : public Piece
{
public:
	Pawn(const Point& position, Team team, const int& currentTurn)
		: Piece{ position, team }, currentTurn{ currentTurn }
	{}


	MoveSet getPossibleMoves(const Board& board, bool getDefenses) const override;
	char getSymbol() const override { return 'p'; }
	std::string_view getName() const override { return "Pawn"; }
	Type getType() const override { return Type::Pawn; }

	// Updates the last time a pawn does a double move
	void setDoubleMove() { turnDoubleMoved = currentTurn; }

	// Resets the last time a pawn did a double move
	void unsetDoubleMove() { turnDoubleMoved = 0; }

	// Gets the last time a pawn double moved
	int getDoubleMove() const { return turnDoubleMoved; }

	// Returns true if this pawn is eligible to be En Passant'd this turn
	bool isPassantable() const { return turnDoubleMoved != 0 && currentTurn == turnDoubleMoved + 1; }

	// Returns the forward direction for this pawn
	constexpr Point forward() const { return { 1 - (2 * team), 0 }; }




private:
	int turnDoubleMoved{};
	const int& currentTurn{};
};