#pragma once
#include <array>
#include "Game/Settings.h"
#include "Game/Point.h"
#include "Piece/PieceEnums.h"

class Board;
class Piece;
class King;

using TeamAttackBoard = std::array<std::array<bool, Settings::g_boardSize>, Settings::g_boardSize>;

class AttackBoard
{
public:
	void update(const Board& board, PieceEnums::Team team, std::array<const Piece*, PieceEnums::MaxTeams> kings);

	// Returns true if the specified team is attacking the point
	bool isAttacking(const Point& point, PieceEnums::Team team) const;


	// For debugging
	void printBoard(std::ostream& out, PieceEnums::Team team) const;
private:
	void resetBoard(PieceEnums::Team team);
	void setNonKing(const Board& board);
	void setKing(const Board& board, const Piece* king);

	// Board to keep track of what tiles are under attack by the enemy
	std::array<TeamAttackBoard, PieceEnums::MaxTeams> attackBoard{};
};