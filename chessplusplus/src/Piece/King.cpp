#include "Piece/King.h"
#include "Board/BoardHelpers.h"
#include "Board/AttackBoard.h"
#include "Board/Board.h"
#include "Piece/PieceEnums.h"

#include "Move/StandardMove.h"
#include "Move/CastlingMove.h"


MoveSet King::getPossibleMoves(const Board& board, bool getDefenses) const
{
    PieceEnums::Team opp{ static_cast<PieceEnums::Team>((team + 1) % PieceEnums::MaxTeams) };
    MoveSet set{};
    for (int dRank = -1; dRank <= 1; ++dRank)
    {
        for (int dFile = -1; dFile <= 1; ++dFile)
        {
            // Can't move onto the same space
            if (dRank == 0 && dFile == 0) { continue; }

            Point dest{ position + Point{dRank, dFile} };

            // King can move onto an unoccupied piece, an enemy piece, or defend an allied piece
            // iff the board isn't under attack (King can't put itself in check)
            if (dest.isInBounds() && (!isOccupied(board, dest) || isEnemyPiece(board, dest, team) || getDefenses) && !attackBoard.isAttacking(dest, opp))
            {
                set.insert({ dest, std::make_unique<StandardMove>(position, dest) });
            }
        }
    }

    // Rules for castling:
    // King cannot be in check
    // The spots the King passes over or lands on cannot be under attack
    // The spots between the Rook and the King are vacant
    // The king and the rooks must be unmoved
    if (!hasMoved && !attackBoard.isAttacking(position, opp))
    {
        Point rookPos{};
        // Check left rook
        rookPos = { position.rank, 0 };

        if (isOccupied(board, rookPos) && board[rookPos]->getType() == PieceEnums::Rook && !board[rookPos]->getMoved()
            && areSpotsUnoccupied(board, position, {0, -1}, 3) && areSpotsUnattacked(position, {0, -1}, 2))
        {
            Point dest{ position.rank, position.file - 2 };
            set.insert({ dest, std::make_unique<CastlingMove>(position, dest, rookPos, dest + Point{0, 1}) });
        }

        // Check right rook
        // Only 2 checks need to be checked for vacancy on this side, not three
        rookPos = { position.rank, Settings::g_boardSize - 1 };
        if (isOccupied(board, rookPos) && board[rookPos]->getType() == PieceEnums::Rook && !board[rookPos]->getMoved()
            && areSpotsUnoccupied(board, position, {0, 1}, 2) && areSpotsUnattacked(position, {0, 1}, 2))
        {
            Point dest{ position.rank, position.file + 2 };
            set.insert({ dest, std::make_unique<CastlingMove>(position, dest, rookPos, dest + Point{0, -1}) });
        }
    }
    return set;
}

bool King::areSpotsUnoccupied(const Board& board, const Point& start, const Point& direction, int num) const
{
    for (int i = 1; i <= num; ++i)
    {
        if (isOccupied(board, start + (direction * i)))
        {
            return false;
        }
    }
    return true;
}

bool King::areSpotsUnattacked(const Point& start, const Point& direction, int num) const
{
    PieceEnums::Team opp{ static_cast<PieceEnums::Team>((team + 1) % PieceEnums::MaxTeams) };
    for (int i = 1; i <= num; ++i)
    {
        if (attackBoard.isAttacking(start + (direction * i), opp))
        {
            return false;
        }
    }
    return true;
}

