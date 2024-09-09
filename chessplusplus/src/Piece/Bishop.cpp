#include "Piece/Bishop.h"

MoveSet Bishop::getPossibleMoves(const Board& board, bool getDefenses) const
{
    return getLineMoves(board, directions, getDefenses);
}

