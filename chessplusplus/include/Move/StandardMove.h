#pragma once
#include "Move/Move.h"

class StandardMove : public Move
{
public:
	StandardMove(const Point& start, const Point& end, bool isAttacking = true)
		: Move{ start, end, isAttacking }
	{}

	MoveResult executeMove(Board& board, std::function<char()> callback) override;
	void undoMove(Board& board) override;

	bool isAttacking() const { return true; }
};