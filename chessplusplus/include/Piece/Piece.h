#pragma once
#include <vector>
#include <iostream>
#include <unordered_map>

#include "Game/Point.h"
#include "Piece/PieceEnums.h"
#include "Move/Move.h"

class Board;

using MoveSet = std::unordered_map<Point, std::unique_ptr<Move>, PointHash>;

using PieceEnums::Team;
using PieceEnums::Type;

class Piece
{
public:
	Piece(const Point& position, Team team)
		: position{ position }, team{ team }
	{}

	void updatePosition(Point newPos) { position = newPos; }

	Point getPosition() const { return position; }

	Team getTeam() const { return team; }

	// Returns true if the piece has moved
	bool getMoved() const { return hasMoved; }

	// Sets the moved state of the piece
	void setMoved(bool state) { hasMoved = state; }

	virtual Type getType() const = 0;

	// Gets all possible moves that can be made by this piece on a given board
	// If getDefenses, the returned MoveSet contains moves that are defending another tile, but can't necessarily be taken currently
	virtual MoveSet getPossibleMoves(const Board& board, bool getDefenses = false) const = 0;

	// Gets the symbol used to represent this piece on the board
	virtual char getSymbol() const = 0;

	virtual std::string_view getName() const = 0;

	// Prints the piece out
	friend std::ostream& operator<<(std::ostream& out, Piece& piece)
	{
		char symbol = piece.getSymbol();
		out << " " << static_cast<char>(static_cast<bool>(piece.team) ? symbol : toupper(symbol)) << " ";
		return out;
	}

protected:
	Point position{};
	Team team{};
	bool hasMoved{};
};