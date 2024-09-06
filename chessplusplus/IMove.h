#pragma once
#include "Point.h"
class Board;

class IMove
{
public:
	virtual void ExecuteMove(Board& board) const = 0;
	virtual ~IMove() = 0;
};