#pragma once
#include <vector>
#include <iostream>

#include "IMove.h"

class Piece
{
public:
	enum Teams
	{
		White,
		Black
	};
	virtual std::vector<IMove> getPossibleMoves() const = 0;
	virtual ~Piece() = 0;

	friend std::ostream& operator<<(std::ostream& out, Piece& piece)
	{
		out << piece.symbol;
		return out;
	}

private:
	char symbol{};
	Point position{};
	int team{};
};