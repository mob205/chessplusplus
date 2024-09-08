#pragma once
#include "Point.h"

class Board;

class Move
{
public:
	virtual void ExecuteMove(Board& board) const = 0;
	Move(const Point& start, const Point& end, bool isAttacking = true)
		: start{ start }, end{ end }, isAttackingMove{isAttacking}
	{}

	bool isAttacking() const { return isAttackingMove; }

protected:
	Point start;
	Point end;

private:
	bool isAttackingMove{ true };
};

