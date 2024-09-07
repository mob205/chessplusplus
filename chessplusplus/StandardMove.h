#pragma once
#include "Move.h"

class StandardMove : public Move
{
public:
	StandardMove(const Point& start, const Point& end)
		: Move{ start, end }
	{}

	void ExecuteMove(Board& board) const override;
};