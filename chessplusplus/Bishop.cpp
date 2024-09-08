#include "Bishop.h"

MoveSet Bishop::getPossibleMoves(const Board& board, bool getDefenses, bool allowRequireExtraInput = false) const
{
    return getLineMoves(board, directions, getDefenses);
}

