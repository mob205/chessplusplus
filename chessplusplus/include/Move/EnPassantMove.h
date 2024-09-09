#pragma once
#include "Move/Move.h"

class EnPassantMove : public Move
{
public:
	EnPassantMove(const Point& start, const Point& end, const Point& captureSpot)
		: Move{ start, end }, captureSpot{ captureSpot }
	{}

	void executeMove(Board& board) override;
	void undoMove(Board& board) override;
	void printMove() const override { std::cout << "EN PASSANT!!!"; }

private:
	Point captureSpot;
};