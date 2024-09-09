#pragma once
#include <functional>
#include <vector>

#include "Board/Board.h"
#include "Piece/Piece.h"
#include "Board/AttackBoard.h"
#include "Game/GameSerializer.h"

class Game
{
	friend GameSerializer;
public:
	Game();
	Board& getBoard() { return board; }
	void playGame();

	GameSerializer& getSerializer() { return serializer; }

private:

	bool hasPossibleNonKingMove(Piece::Team team);

	bool isInCheck(Piece::Team team);

	bool processTurn(const Point& start, const Point& end, bool printMove, std::function<char()> getExtraInput);

	std::vector<std::unique_ptr<Move>> moveHistory{};

	// Non-owning pointers to view kings
	std::array<const Piece*, Piece::MaxTeams> kings;

	Board board{};
	AttackBoard attackBoard{};

	GameSerializer serializer{ *this };

	int currentTurn{};
	Piece::Team currentTeam{Piece::White};
};

