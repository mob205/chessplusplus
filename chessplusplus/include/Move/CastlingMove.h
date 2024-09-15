#pragma once
#include "Move/Move.h"

class CastlingMove : public Move
{
public:
	CastlingMove(const Point& kingStart, const Point& kingEnd, const Point& rookStart, const Point& rookEnd)
		: Move{ kingStart, kingEnd, false }, rookStart{ rookStart }, rookEnd{ rookEnd }
	{}

	MoveResult executeMove(Board& board, char extraInput = '\0') override;
	void undoMove(Board& board) override;

private:
	Point rookStart{};
	Point rookEnd{};
};