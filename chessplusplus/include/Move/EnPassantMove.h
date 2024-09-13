#pragma once
#include "Move/Move.h"

class EnPassantMove : public Move
{
public:
	EnPassantMove(const Point& start, const Point& end, const Point& captureSpot)
		: Move{ start, end }, captureSpot{ captureSpot }
	{}

	MoveResult executeMove(Board& board, std::function<char()> callback) override;
	void undoMove(Board& board) override;

private:
	Point captureSpot;
};