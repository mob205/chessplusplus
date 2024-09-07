#pragma once
#include "Move.h"
#include "Pawn.h"

class DoublePawnMove : public Move
{
public:
	DoublePawnMove(const Point& start, const Point& end)
		: Move{ start, end }
	{}

	void ExecuteMove(Board& board) const override;
};
