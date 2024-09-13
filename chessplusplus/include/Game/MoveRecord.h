#pragma once
#include <memory>

#include "Move/MoveResult.h"
class Move;

struct MoveRecord
{
	// Owned pointer to store all the state necessary for undoing the move
	std::unique_ptr<Move> move{};

	MoveResult result{};
};