#pragma once

#include "Piece.h"

class Pawn : public Piece
{
public:
	Pawn(Point position, Team team)
		: Piece{ position, team }
	{}

	MoveSet getPossibleMoves(Board& board) const override;
	char getSymbol() const override { return team ? 'p' : toupper('p'); }
	bool isPassantable(int curTurn) const { turnDoubleMoved != 0 && curTurn == turnDoubleMoved + 1; }

private:
	constexpr Point forward() const { return { 0, 1 - (2 * team) }; }
	int turnDoubleMoved{};
};