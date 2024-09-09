#pragma once
#include <array>
#include "Piece/LinePiece.h"

class Rook : public LinePiece
{
public:
	Rook(const Point& position, Team team)
		: LinePiece{ position, team }
	{}

	MoveSet getPossibleMoves(const Board& board, bool getDefenses) const override;
	char getSymbol() const override { return 'r'; }
	std::string_view getName() const override { return "Rook"; }
	Type getType() const override { return Type::Rook; }

private:
	static constexpr std::array directions
	{
		Point{1, 0},
		Point{0, 1},
		Point{-1, 0},
		Point{0, -1},
	};
};