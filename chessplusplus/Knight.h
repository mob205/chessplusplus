#pragma once
#include "Piece.h"

#include <array>




class Knight : public Piece
{
public:

	Knight(const Point& position, Team team)
		: Piece{ position, team }
	{}

	MoveSet getPossibleMoves(Board& board) const override;

	char getSymbol() const override { return 'k'; }

	std::string_view getName() const override { return "Knight"; }

private:
	static constexpr std::array directions{
		Point{2, 1},
		Point{1, 2},
		Point{2, -1},
		Point{-1, 2},
		Point{-2, -1},
		Point{-1, -2},
		Point{-2, 1},
		Point{1, -2},
	};
};