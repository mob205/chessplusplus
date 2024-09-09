#pragma once
#include "Move/Move.h"

class StandardMove : public Move
{
public:
	StandardMove(const Point& start, const Point& end, bool isAttacking = true)
		: Move{ start, end, isAttacking }
	{}

	void executeMove(Board& board) override;
	void undoMove(Board& board) override;
	void printMove() const override;

	bool isAttacking() const { return true; }
};