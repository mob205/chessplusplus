#pragma once
#include <vector>
#include <iostream>
#include <unordered_map>

#include "Move.h"

class Board;

using MoveSet = std::unordered_map<Point, std::unique_ptr<Move>, PointHash>;

class Piece
{
public:
	enum Team
	{
		White,
		Black,
		MaxTeams
	};

	Piece(Point position, Team team)
		: position{ position }, team{ team }
	{}

	void updatePosition(Point newPos) { position = newPos; }

	Team getTeam() const { return team; }

	// Returns true if the piece has moved
	bool getMoved() const { return hasMoved; }

	// Sets the moved state of the piece
	void setMoved(bool state) { hasMoved = state; }

	// Gets all possible moves that can be made by this piece on a given board
	virtual MoveSet getPossibleMoves(Board& board) const = 0;

	// Gets the symbol used to represent this piece on the board
	virtual char getSymbol() const = 0;

	virtual std::string_view getName() const = 0;

	// Prints the piece out
	friend std::ostream& operator<<(std::ostream& out, Piece& piece)
	{
		out << " " << piece.getSymbol() << " ";
		return out;
	}

protected:
	Point position{};
	Team team{};
	bool hasMoved{};
};