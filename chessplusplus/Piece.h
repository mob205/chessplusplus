#pragma once
#include <vector>
#include <iostream>

#include "IMove.h"

class Board;

class Piece
{
public:
	enum Team
	{
		White,
		Black
	};
	Piece(Point position, Team team)
		: position{ position }, team{ team }
	{}
	void UpdatePosition(Point newPos) { position = newPos; }

	virtual std::vector<IMove> getPossibleMoves(Board& board) const = 0;
	virtual char getSymbol() const = 0;

	friend std::ostream& operator<<(std::ostream& out, Piece& piece)
	{
		out << piece.getSymbol();
		return out;
	}

protected:
	Point position{};
	Team team{};
};