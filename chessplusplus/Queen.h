#pragma once
#include <array>
#include "LinePiece.h"

class Queen : public LinePiece
{
public:
	Queen(const Point& position, Team team)
		: LinePiece{ position, team }
	{}

	MoveSet getPossibleMoves(const Board& board, bool getDefenses) const override;
	char getSymbol() const override { return 'q'; }
	std::string_view getName() const override { return "Q"; }
	Type getType() const override { return Type::Queen; }

private:
	static constexpr std::array directions
	{
		// Straights
		Point{1, 0},
		Point{0, 1},
		Point{-1, 0},
		Point{0, -1},

		// Diagonals
		Point{1, 1},
		Point{-1, 1},
		Point{-1, -1},
		Point{1, -1},
	};
};