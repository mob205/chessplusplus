#pragma once

#include "Board.h"
#include "Piece.h"
#include "AttackBoard.h"

class Game
{
	friend void playGame(Game& game);

public:
	Game();
	Board& getBoard() { return board; }


private:
	// Non-owning pointers to view kings
	std::array<const Piece*, Piece::MaxTeams> kings;

	Board board{};

	// attackBoards[team] gives the board representing tiles that the team is attacking
	AttackBoard attackBoard{};

	int currentTurn{};
	Piece::Team currentTeam{Piece::White};
};

