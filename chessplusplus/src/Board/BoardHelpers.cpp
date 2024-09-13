#include <memory>

#include "Board/BoardHelpers.h"
#include "Game/Point.h"
#include "Board/Board.h"
#include "Piece/Piece.h"
#include "Piece/PieceEnums.h"

bool isOccupied(const Board& board, const Point& point)
{
	return static_cast<bool>(board[point]);
}

bool ownsPiece(const Board& board, const Point& point, PieceEnums::Team team)
{
	return isOccupied(board, point) && board[point]->getTeam() == team;
}

bool isEnemyPiece(const Board& board, const Point& point, PieceEnums::Team team)
{
	return isOccupied(board, point) && board[point]->getTeam() != team;
}

bool isAlliedPiece(const Board& board, const Point& point, PieceEnums::Team team)
{
	return isOccupied(board, point) && board[point]->getTeam() == team;
}

bool isType(const Board& board, const Point& point, PieceEnums::Type type)
{
	return isOccupied(board, point) && board[point]->getType() == type;
}