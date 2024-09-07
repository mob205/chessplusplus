#include "Pawn.h"
#include "Board.h"
#include "BoardHelpers.h"

#include "StandardMove.h"
#include "DoublePawnMove.h"
#include "EnPassantMove.h"

MoveSet Pawn::getPossibleMoves(Board& board) const
{
	MoveSet set{};
	Point ahead{ position + forward() };

	// Forward move
	if (ahead.isInBounds() && !isOccupied(board, ahead))
	{
		set.insert({ ahead, std::make_unique<StandardMove>(position, ahead) });
	}
	
	// Double forward move - pawn's first move only
	Point doubleAhead{ ahead + forward() };
	// Should be impossible for ahead or doubleAhead to be out-of-bounds if the piece hasn't moved
	if (!hasMoved && !isOccupied(board, ahead) && !isOccupied(board, doubleAhead))
	{
		set.insert({ doubleAhead, std::make_unique<DoublePawnMove>(position, doubleAhead) });
	}

	for (int i = -1; i <= 1; i += 2)
	{
		Point side{ position + Point{0, i} };
		Point forwardSide{ forward() + side};

		// Diagonal capture
		if (forwardSide.isInBounds() && isEnemyPiece(board, forwardSide, team))
		{
			set.insert({ forwardSide, std::make_unique<StandardMove>(position, forwardSide) });

			// En passant isn't even possible if we can diagonal capture, so just continue
			continue;
		}

		// En passant
		if (side.isInBounds() && isEnemyPiece(board, side, team))
		{
			try
			{
				Pawn* sidePawn{ dynamic_cast<Pawn*>(board[side].get()) };
				if (sidePawn->isPassantable())
				{
					set.insert({ forwardSide, std::make_unique<EnPassantMove>(position, forwardSide, side) });
				}
			}
			catch (std::bad_cast) {}
		}
	}

	return set;
}