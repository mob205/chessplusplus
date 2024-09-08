#pragma once
#include <stack>

#include "Board.h"
#include "Piece.h"
#include "AttackBoard.h"

class Game
{
	friend void playGame(Game& game);

public:
	Game();
	Board& getBoard() { return board; }
	void playGame();

private:

	bool hasPossibleNonKingMove(Piece::Team team);

	bool isInCheck(Piece::Team team);

	std::stack<std::unique_ptr<Move>> moveHistory{};

	// Non-owning pointers to view kings
	std::array<const Piece*, Piece::MaxTeams> kings;

	Board board{};

	AttackBoard attackBoard{};

	int currentTurn{};
	Piece::Team currentTeam{Piece::White};
};

