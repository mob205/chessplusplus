#pragma once
#include <array>
#include "Settings.h"
#include "Point.h"
#include "Piece.h"

class Board;
class King;

using TeamAttackBoard = std::array<std::array<bool, Settings::g_boardSize>, Settings::g_boardSize>;

class AttackBoard
{
public:
	void update(const Board& board, Piece::Team team, int turn, std::array<const Piece*, Piece::MaxTeams> kings);

	bool isAttacked(const Point& point, Piece::Team team) const;


	// For debugging
	friend std::ostream& operator<<(std::ostream& out, const AttackBoard& attackBoard);
private:
	void resetBoard(Piece::Team team);
	void setNonKing(const Board& board);
	void setKing(const Board& board, const Piece* king);

	int currentTurn{-1};

	// Board to keep track of what tiles are under attack by the enemy
	std::array<TeamAttackBoard, Piece::MaxTeams> attackBoard{};
};