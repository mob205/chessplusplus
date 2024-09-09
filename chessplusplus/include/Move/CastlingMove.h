#pragma once
#include "Move/Move.h"

class CastlingMove : public Move
{
	public:
	CastlingMove(const Point& kingStart, const Point& kingEnd, const Point& rookStart, const Point& rookEnd)
		: Move{ kingStart, kingEnd, false }, rookStart{ rookStart }, rookEnd{ rookEnd }
	{}

	void executeMove(Board& board) override;
	void undoMove(Board& board) override;
	void printMove() const override;

private:
	Point rookStart{};
	Point rookEnd{};
};