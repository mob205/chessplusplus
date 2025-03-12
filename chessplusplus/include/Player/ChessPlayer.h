#pragma once

#include "Game/Point.h"
#include "Move/MoveInput.h"
#include "Piece/PieceEnums.h"

class Game;

class ChessPlayer
{
public:

	ChessPlayer(PieceEnums::Team team)
		: team{ team }
	{};

	// Alerts the chess player of start of turn
	// Start move generation here
	virtual void startTurn(Game* game) {};

	// Returns true if the player has a move ready
	// This move is stored in outMove
	virtual bool getMove(MoveInput& outMove) { return false; }

	// Returns true if this player type should allow move input on local GUI
	virtual bool usesGUI() { return true; }

	// Called when the local player selects a move via the GUI
	virtual void onLocalMoveSelected(MoveInput move) {};

	virtual ~ChessPlayer() = default;

protected:
	PieceEnums::Team team;


public:
	PieceEnums::Team getTeam() const { return team; }
};