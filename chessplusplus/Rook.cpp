#include "Rook.h"

MoveSet Rook::getPossibleMoves(const Board& board, bool getDefenses, bool allowRequireExtraInput) const
{
    return getLineMoves(board, directions, getDefenses);
}

