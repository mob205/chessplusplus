#include "BoardHelpers.h"
#include "Point.h"
#include "Board.h"
#include "Piece.h"

bool isOccupied(const Board& board, const Point& point)
{
	return static_cast<bool>(board[point]);
}

bool ownsPiece(const Board& board, const Point& point, Piece::Team team)
{
	return isOccupied(board, point) && board[point]->getTeam() == team;
}

bool isEnemyPiece(const Board& board, const Point& point, Piece::Team team)
{
	return isOccupied(board, point) && board[point]->getTeam() != team;
}

bool isAlliedPiece(const Board& board, const Point& point, Piece::Team team)
{
	return isOccupied(board, point) && board[point]->getTeam() == team;
}

bool isType(const Board& board, const Point& point, Piece::Type type)
{
	return isOccupied(board, point) && board[point]->getType() == type;
}