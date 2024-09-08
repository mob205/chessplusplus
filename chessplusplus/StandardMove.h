#pragma once
#include "Move.h"

class StandardMove : public Move
{
public:
	StandardMove(const Point& start, const Point& end, bool isAttacking = true)
		: Move{ start, end, isAttacking }
	{}

	void ExecuteMove(Board& board) const override;

	bool isAttacking() const { return true; }
};