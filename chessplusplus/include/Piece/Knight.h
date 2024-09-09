#pragma once
#include "Piece/Piece.h"

#include <array>




class Knight : public Piece
{
public:

	Knight(const Point& position, Team team)
		: Piece{ position, team }
	{}

	MoveSet getPossibleMoves(const Board& board, bool getDefenses, bool allowRequireExtraInput) const override;
	char getSymbol() const override { return 'n'; }
	std::string_view getName() const override { return "Knight"; }
	Type getType() const override { return Type::Knight; }

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