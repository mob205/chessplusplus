#pragma once
#include <array>
#include "Piece.h"

class AttackBoard;

class King : public Piece
{
public:
	King(const Point& position, Team team, const AttackBoard& attackBoard)
		: Piece{position, team}, attackBoard{attackBoard}
	{}
	MoveSet getPossibleMoves(const Board& board, bool getDefenses, bool allowRequireExtraInput) const override;
	char getSymbol() const override { return 'k'; }
	std::string_view getName() const override { return "King"; }
	Type getType() const override { return Type::King; }

private:
	const AttackBoard& attackBoard;

	bool areSpotsUnoccupied(const Board& board, const Point& start, const Point& dir, int num) const;
	bool areSpotsUnattacked(const Point& start, const Point& dir, int num) const;
};