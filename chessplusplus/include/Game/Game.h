#pragma once
#include <functional>
#include <vector>

#include "Board/Board.h"
#include "Piece/Piece.h"
#include "Piece/PieceEnums.h"

#include "Board/AttackBoard.h"
#include "Game/GameSerializer.h"
#include "Game/MoveRecord.h"

class Game
{
	friend GameSerializer;
public:
	Game();
	Board& getBoard() { return board; }
	void playGame();

	GameSerializer& getSerializer() { return serializer; }

	MoveResult processTurn(const Point& start, const Point& end, bool printMove, std::function<char()> getExtraInput);

private:

	bool hasPossibleNonKingMove(PieceEnums::Team team);

	bool isInCheck(PieceEnums::Team team);

	std::vector<MoveRecord> moveHistory{};

	// Non-owning pointers to view kings
	std::array<const Piece*, PieceEnums::MaxTeams> kings;

	Board board{};
	AttackBoard attackBoard{};

	GameSerializer serializer{ *this };

	int currentTurn{};
	PieceEnums::Team currentTeam{PieceEnums::White};
};

PieceEnums::Team getOppositeTeam(PieceEnums::Team team);
