#include "Rook.h"

MoveSet Rook::getPossibleMoves(const Board& board, bool getDefenses) const
{
    return getLineMoves(board, directions, getDefenses);
}

