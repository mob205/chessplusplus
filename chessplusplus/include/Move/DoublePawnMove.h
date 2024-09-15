#pragma once
#include "Move/Move.h"

class DoublePawnMove : public Move
{
public:
	DoublePawnMove(const Point& start, const Point& end, bool isAttacking = false)
		: Move{ start, end, false }
	{}

	MoveResult executeMove(Board& board, char extraInput = '\0') override;
	void undoMove(Board& board) override;
};
