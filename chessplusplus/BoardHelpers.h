#pragma once
#include "Point.h"
#include "Board.h"

bool isOccupied(const Board& board, const Point& point)
{
	return board[point] != nullptr;
}
