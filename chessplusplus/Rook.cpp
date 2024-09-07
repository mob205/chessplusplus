#include "Rook.h"

MoveSet Rook::getPossibleMoves(Board& board) const
{
    return getLineMoves(board, directions);
}

