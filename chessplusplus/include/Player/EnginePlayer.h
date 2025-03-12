#pragma once

#include "ChessPlayer.h"
#include "Game/Game.h"
#include <future>

class EnginePlayer : public ChessPlayer
{
public:
	EnginePlayer(PieceEnums::Team team)
		: ChessPlayer{ team }
	{}

	virtual void startTurn(const Game& game) override;

	virtual bool getMove(MoveInput& outMove) override;

	virtual bool usesGUI() override { return false; }

private:
	std::future<MoveInput> engineThread{};

	std::unique_ptr<Game> storedGame{};
};