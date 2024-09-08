#pragma once
#include "Move.h"
#include "Pawn.h"

class DoublePawnMove : public Move
{
public:
	DoublePawnMove(const Point& start, const Point& end, bool isAttacking = false)
		: Move{ start, end, false }
	{}

	void ExecuteMove(Board& board) const override;
};
