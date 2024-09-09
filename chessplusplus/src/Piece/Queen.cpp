#include "Piece/Queen.h"

MoveSet Queen::getPossibleMoves(const Board& board, bool getDefenses, bool allowRequireExtraInput) const
{
    return getLineMoves(board, directions, getDefenses);
}

