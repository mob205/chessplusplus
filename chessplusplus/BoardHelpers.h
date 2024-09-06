#pragma once
#include "Point.h"
#include "Board.h"

inline bool isOccupied(const Board& board, const Point& point)
{
	return board[point] != nullptr;
}

inline bool ownsPiece(const Board& board, const Point& point, Piece::Team team)
{
	return isOccupied(board, point) && board[point]->getTeam() == team;
}
