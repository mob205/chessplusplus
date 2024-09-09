#pragma once
#include "Piece/Piece.h"
struct Point;
class Board;

bool isOccupied(const Board& board, const Point& point);

bool ownsPiece(const Board& board, const Point& point, Piece::Team team);

bool isEnemyPiece(const Board& board, const Point& point, Piece::Team team);

bool isAlliedPiece(const Board& board, const Point& point, Piece::Team team);

bool isType(const Board& board, const Point& point, Piece::Type type);