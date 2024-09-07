#pragma once
#include "Point.h"
#include "Board.h"

inline bool isOccupied(const Board& board, const Point& point)
{
	return static_cast<bool>(board[point]);
}

inline bool ownsPiece(const Board& board, const Point& point, Piece::Team team)
{
	return isOccupied(board, point) && board[point]->getTeam() == team;
}

inline bool isEnemyPiece(const Board& board, const Point& point, Piece::Team team)
{
	return isOccupied(board, point) && board[point]->getTeam() != team;
}
