#include "Knight.h"
#include "BoardHelpers.h"
#include "StandardMove.h"

MoveSet Knight::getPossibleMoves(const Board& board, bool getDefenses) const
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

