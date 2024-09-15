#pragma once
#include <functional>
#include "Game/Point.h"

#include "Move/MoveResult.h"

class Piece;
class Board;

class Move
{
public:
	virtual MoveResult executeMove(Board& board, char extraInput = '\0') = 0;

	virtual void undoMove(Board& board) = 0;

	Move(const Point& start, const Point& end, bool isAttacking = true)
		: start{ start }, end{ end }, isAttackingMove{isAttacking}
	{}

	bool isAttacking() const { return isAttackingMove; }

	Point getStart() const { return start; }
	Point getEnd() const { return end; }

	char getExtraInput() const { return extraInput; }

protected:
	Point start;
	Point end;

	// Save the status of whether the moved piece had taken its first move prior to this move or not for undo
	bool originalMoved{};
	std::unique_ptr<Piece> captured{};

	// Store extra input to be saved later, if needed
	char extraInput{};

private:
	bool isAttackingMove{ true };
};

