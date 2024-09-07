#pragma once
#include "Point.h"

class Board;

class Move
{
public:
	virtual void ExecuteMove(Board& board) const = 0;
	Move(const Point& start, const Point& end)
		: start{ start }, end{ end }
	{}

protected:
	Point start;
	Point end;
};

