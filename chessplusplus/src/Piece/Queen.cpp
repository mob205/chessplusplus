#include "Piece/Queen.h"

MoveSet Queen::getPossibleMoves(const Board& board, bool getDefenses) const
{
    return getLineMoves(board, directions, getDefenses);
}

