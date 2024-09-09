#include "Piece/Knight.h"
#include "Board/BoardHelpers.h"
#include "Move/StandardMove.h"

MoveSet Knight::getPossibleMoves(const Board& board, bool getDefenses, bool allowRequireExtraInput) const
{
    MoveSet set{};
    for (const auto& dir : directions)
    {
        Point dest{ dir + position };
        if (dest.isInBounds() && !isAlliedPiece(board, dest, team))
        {
            set.insert({ dest, std::make_unique<StandardMove>(position, dest) });
        }
    }
    return set;
}

