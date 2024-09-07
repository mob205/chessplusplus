#pragma once
#include <array>
#include "LinePiece.h"

class Bishop : public LinePiece
{
public:
	Bishop(const Point& position, Team team)
		: LinePiece{ position, team }
	{}

	MoveSet getPossibleMoves(Board& board) const override;
	char getSymbol() const override { return 'b'; }
	std::string_view getName() const override { return "Bishop"; }

private:
	static constexpr std::array directions
	{
		Point{1, 1},
		Point{-1, 1},
		Point{-1, -1},
		Point{1, -1},
	};
};