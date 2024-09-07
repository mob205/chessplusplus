#include "Bishop.h"

MoveSet Bishop::getPossibleMoves(Board& board) const
{
    return getLineMoves(board, directions);
}

