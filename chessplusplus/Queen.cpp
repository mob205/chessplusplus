#include "Queen.h"

MoveSet Queen::getPossibleMoves(Board& board) const
{
    return getLineMoves(board, directions);
}

