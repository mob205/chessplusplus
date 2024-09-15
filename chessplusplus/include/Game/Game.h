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
	// Sets up initial chess board
	Game();
	
	// Returns reference to game board
	const Board& getBoard() const { return board; }

	// Returns the current turn
	int getCurrentTurn() const { return currentTurn; }
	
	// Returns the team whose turn it currently is
	PieceEnums::Team getCurrentTeam() const { return static_cast<PieceEnums::Team>((currentTurn) % PieceEnums::MaxTeams); }

	// Returns this game's serializer
	GameSerializer& getSerializer() { return serializer; }

	// Processes a turn given valid start and end inputs
	MoveResult processTurn(const Point& start, const Point& end, std::function<char()> getExtraInput);

	// Undos the most recent move
	bool undoMove();

private:
	// Returns true if this team has a possible move from any non-king piece or pawn
	// Returns false otherwise
	bool hasPossibleNonKingMove(PieceEnums::Team team);

	// Returns true if this team's king is in check and false otherwise
	bool isInCheck(PieceEnums::Team team, const AttackBoard& attackBoard) const;
	bool isInCheck(PieceEnums::Team team) const;

	// Analyzes board state for check, checkmate, and stalemate
	MoveResult::OpponentStatus checkEndConditions();

	std::vector<MoveRecord> moveHistory{};

	// Non-owning pointers to view kings
	std::array<const Piece*, PieceEnums::MaxTeams> kings;

	Board board{};
	AttackBoard attackBoard{};

	GameSerializer serializer{ *this };

	int currentTurn{};
};

