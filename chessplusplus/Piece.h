#pragma once
#include <vector>
#include <iostream>
#include <unordered_map>

#include "IMove.h"

class Board;

using MoveSet = std::unordered_map<Point, std::unique_ptr<IMove>, PointHash>;

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

	void updatePosition(Point newPos) { position = newPos; }
	Team getTeam() const { return team; }

	virtual MoveSet getPossibleMoves(Board& board) const = 0;
	virtual char getSymbol() const = 0;

	friend std::ostream& operator<<(std::ostream& out, Piece& piece)
	{
		out << " " << piece.getSymbol() << " ";
		return out;
	}

protected:
	Point position{};
	Team team{};
};