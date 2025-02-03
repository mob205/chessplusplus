#pragma once
#include <array>
#include "Game/Settings.h"
#include "Game/Point.h"
#include "Piece/PieceEnums.h"

class Board;
class Piece;
class King;

using TeamAttackBoard = std::array<std::array<bool, Settings::boardSize>, Settings::boardSize>;

// A specialized board to keep track of what tiles are under attack by the enemy
class AttackBoard
{
public:
	void update(const Board& board, PieceEnums::Team team, std::array<const Piece*, PieceEnums::MaxTeams> kings);

	// Returns true if the specified team is attacking the point
	bool isAttacking(const Point& point, PieceEnums::Team team) const;


	// For debugging
	void printBoard(std::ostream& out, PieceEnums::Team team) const;
private:

	// Resets the attack board to an initial state showing no tiles being attacked
	void resetBoard(PieceEnums::Team team);

	// Sets the attack board to represent all board tiles being attacked by given team
	void setNonKing(const Board& board);

	// Sets the attack board to represent spaces where a king can attack. Must be done after other pieces have been set
	void setKing(const Board& board, const Piece* king);

	std::array<TeamAttackBoard, PieceEnums::MaxTeams> attackBoard{};
};