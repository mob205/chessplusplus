#pragma once
#include "Move/Move.h"

class DoublePawnMove : public Move
{
public:
	DoublePawnMove(const Point& start, const Point& end, bool isAttacking = false)
		: Move{ start, end, false }
	{}

	void executeMove(Board& board, std::function<char()> callback) override;
	void undoMove(Board& board) override;
	void printMove() const override {};
};
