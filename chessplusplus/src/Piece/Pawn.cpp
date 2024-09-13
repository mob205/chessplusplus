#include "Piece/Pawn.h"
#include "Piece/PieceEnums.h"

#include "Board/Board.h"
#include "Board/BoardHelpers.h"

#include "Move/PawnMove.h"
#include "Move/DoublePawnMove.h"
#include "Move/EnPassantMove.h"

MoveSet Pawn::getPossibleMoves(const Board& board, bool getDefenses) const
{
	MoveSet set{};
	Point ahead{ position + forward() };

	// Forward move
	if (ahead.isInBounds() && !isOccupied(board, ahead))
	{
		set.insert({ ahead, std::make_unique<PawnMove>(position, ahead, false) });
	}
	
	// Double forward move - pawn's first move only
	Point doubleAhead{ ahead + forward() };
	// Should be impossible for ahead or doubleAhead to be out-of-bounds if the piece hasn't moved
	if (!hasMoved && !isOccupied(board, ahead) && !isOccupied(board, doubleAhead))
	{
		set.insert({ doubleAhead, std::make_unique<DoublePawnMove>(position, doubleAhead, false) });
	}

	for (int i = -1; i <= 1; i += 2)
	{
		Point side{ position + Point{0, i} };
		Point forwardSide{ forward() + side};

		// Diagonal capture
		if (forwardSide.isInBounds() && (isEnemyPiece(board, forwardSide, team) || getDefenses))
		{
			set.insert({ forwardSide, std::make_unique<PawnMove>(position, forwardSide, true) });

			// En passant isn't even possible if we can diagonal capture, so just continue
			continue;
		}

		// En passant
		if (side.isInBounds() && isEnemyPiece(board, side, team) && isType(board, side, PieceEnums::Pawn))
		{
			Pawn* sidePawn{ static_cast<Pawn*>(board[side].get())};
			if (sidePawn->isPassantable())
			{
				set.insert({ forwardSide, std::make_unique<EnPassantMove>(position, forwardSide, side) });
			}
		}
	}

	return set;
}