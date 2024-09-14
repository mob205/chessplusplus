#include "Board/AttackBoard.h"
#include "Board/BoardHelpers.h"
#include "Board/Board.h"

#include "Piece/Piece.h"
#include "Piece/PieceEnums.h"

#include "Move/Move.h"

void AttackBoard::update(const Board& board, PieceEnums::Team team, std::array<const Piece*, PieceEnums::MaxTeams> kings)
{
	// Same turn, so the attack board is still valid.

	for (int i = 0; i < PieceEnums::MaxTeams; ++i)
	{
		resetBoard(static_cast<PieceEnums::Team>(i));
	}
	
	// Set where regular pieces are attacking, which will limit the kings' movements
	setNonKing(board);

	// Set the opposing team king can attack first, since it will limit the current team's king's movement 
	setKing(board, kings[getOppositeTeam(team)]);

	// Where the current king is attacking shouldn't matter for current turn
	//setKing(board, kings[team]);
}

bool AttackBoard::isAttacking(const Point& point, PieceEnums::Team team) const
{
	return attackBoard[team][point.rank][point.file];
}

// Resets the attack board to an initial state showing no tiles being attacked
void AttackBoard::resetBoard(PieceEnums::Team team)
{
	std::fill(attackBoard[team].begin(), attackBoard[team].end(), std::array<bool, Settings::g_boardSize>{});
}

// Sets the attack board to represent all board tiles being attacked by given team
void AttackBoard::setNonKing(const Board& board)
{
	for (int rank = 0; rank < Settings::g_boardSize; ++rank)
	{
		for (int file = 0; file < Settings::g_boardSize; ++file)
		{
			Point pos{ rank, file };

			// Can only be attacked by enemy pieces
			// King's moveset relies on the attackBoard, so don't check it while making it
			if (!isOccupied(board, pos) || isType(board, pos, PieceEnums::King)) { continue; }
		
			MoveSet set{ board[pos]->getPossibleMoves(board, true) };

			// Get the team board associated with the piece
			TeamAttackBoard& teamBoard{ attackBoard[board[pos]->getTeam()] };
			for (const auto& move : set)
			{
				if (!move.second->isAttacking()) { continue; }
				teamBoard[move.first.rank][move.first.file] = true;
			}
		}
	}
}

// Sets the attack board to represent spaces where a king can attack. Must be done after other pieces have been set
void AttackBoard::setKing(const Board& board, const Piece* king)
{
	MoveSet set{ king->getPossibleMoves(board, true) };
	TeamAttackBoard& teamBoard{ attackBoard[king->getTeam()] };

	for (const auto& move : set)
	{
		teamBoard[move.first.rank][move.first.file] = true;
	}
}

void AttackBoard::printBoard(std::ostream& out, PieceEnums::Team team) const
{
	out << "   ";
	for (int i = 0; i < Settings::g_boardSize; ++i)
	{
		out << " " << static_cast<char>('a' + i) << " ";
	}

	out << "\n   ";
	for (int i = 0; i < Settings::g_boardSize; ++i)
	{
		out << "---";
	}
	out << "\n";
	for (int rank = Settings::g_boardSize - 1; rank >= 0; --rank)
	{
		out << rank + 1 << " |";
		for (int file = 0; file < Settings::g_boardSize; ++file)
		{
			if (this->isAttacking({ rank, file }, team))
			{
				out << " X ";
			}
			else
			{
				out << "   ";
			}
		}
		out << "|\n";
	}
	out << "   ";
	for (int i = 0; i < Settings::g_boardSize; ++i)
	{
		out << "---";
	}
	out << "\n";
}