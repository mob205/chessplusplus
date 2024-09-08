#include "King.h"
#include "BoardHelpers.h"
#include "StandardMove.h"
#include "AttackBoard.h"


MoveSet King::getPossibleMoves(const Board& board, bool getDefenses, bool allowRequireExtraInput) const
{
    Piece::Team opp{ static_cast<Piece::Team>((team + 1) % Piece::MaxTeams) };
    MoveSet set{};
    for (int dRank = -1; dRank <= 1; ++dRank)
    {
        for (int dFile = -1; dFile <= 1; ++dFile)
        {
            // Can't move onto the same space
            if (dRank == 0 && dFile == 0) { continue; }

            Point dest{ position + Point{dRank, dFile} };

            // Situational case: King defends the allied piece
            if (dest.isInBounds() && (!isOccupied(board, dest) || isEnemyPiece(board, dest, team) || getDefenses) && !attackBoard.isAttacking(dest, opp))
            {
                set.insert({ dest, std::make_unique<StandardMove>(position, dest) });
            }
        }
    }
    return set;
}

