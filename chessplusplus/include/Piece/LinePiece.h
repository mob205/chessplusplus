#pragma once

#include "Piece/Piece.h"
#include "Board/BoardHelpers.h"
#include "Move/StandardMove.h"

class LinePiece : public Piece
{
public:
	LinePiece(Team team)
		: Piece{ team }
	{}

protected:
	template <int N>
	MoveSet getLineMoves(const Board& board, std::array<Point, N> directions, bool getDefenses) const
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
			// Last spot in the line is a piece - can capture enemies piece, but can't move past it
			// Can also defend allies
			if (dest.isInBounds() && (isEnemyPiece(board, dest, team) || getDefenses))
			{
				set.insert({ dest, std::make_unique<StandardMove>(position, dest) });
			}
		}
		return set;
	}
};