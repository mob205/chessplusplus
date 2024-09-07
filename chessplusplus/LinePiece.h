#pragma once

#include "Piece.h"
#include "BoardHelpers.h"
#include "StandardMove.h"

class LinePiece : public Piece
{
public:
	LinePiece(const Point& position, Team team)
		: Piece{ position, team }
	{}

protected:
	template <int N>
	MoveSet getLineMoves(Board& board, std::array<Point, N> directions) const
	{
		MoveSet set{};
		for (int i = 0; i < N; ++i)
		{
			Point direction{ directions[i] };
			Point dest{ position + direction};
			
			// All unoccupied spaces in that direction
			while (dest.isInBounds() && !isOccupied(board, dest))
			{
				set.insert({ dest, std::make_unique<StandardMove>(position, dest) });
				dest += direction;
			}
			// Last spot in the line is an enemy piece - can capture that piece, but can't move past it
			if (dest.isInBounds() && isEnemyPiece(board, dest, team))
			{
				set.insert({ dest, std::make_unique<StandardMove>(position, dest) });
			}
		}
		return set;
	}
};