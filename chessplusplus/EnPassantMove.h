#pragma once
#include "Move.h"

class EnPassantMove : public Move
{
public:
	EnPassantMove(const Point& start, const Point& end, const Point& captureSpot)
		: Move{ start, end }, captureSpot{ captureSpot }
	{}

	void ExecuteMove(Board& board) const override;

private:
	Point captureSpot;
};