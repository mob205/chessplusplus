#pragma once
#include "Piece/PieceEnums.h"

struct Point;
class Board;

bool isOccupied(const Board& board, const Point& point);

bool ownsPiece(const Board& board, const Point& point, PieceEnums::Team team);

bool isEnemyPiece(const Board& board, const Point& point, PieceEnums::Team team);

bool isAlliedPiece(const Board& board, const Point& point, PieceEnums::Team team);

bool isType(const Board& board, const Point& point, PieceEnums::Type type);

PieceEnums::Team getOppositeTeam(PieceEnums::Team team);
