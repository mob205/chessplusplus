#pragma once
#include "Point.h"

class Piece;
class Board;

class Move
{
public:
	virtual void executeMove(Board& board) = 0;

	virtual void undoMove(Board& board) = 0;

	Move(const Point& start, const Point& end, bool isAttacking = true)
		: start{ start }, end{ end }, isAttackingMove{isAttacking}
	{}

	bool isAttacking() const { return isAttackingMove; }

	virtual void printMove() const = 0;

protected:
	Point start;
	Point end;

	bool originalMoved{};
	std::unique_ptr<Piece> captured{};

private:
	bool isAttackingMove{ true };
};

