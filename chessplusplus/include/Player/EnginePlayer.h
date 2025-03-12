#pragma once

#include "ChessPlayer.h"
#include <future>

class EnginePlayer : public ChessPlayer
{
public:
	EnginePlayer(PieceEnums::Team team)
		: ChessPlayer{ team }
	{}

	virtual void startTurn(Game* game) override;

	virtual bool getMove(MoveInput& outMove) override;

	virtual bool usesGUI() override { return false; }

private:
	std::future<MoveInput> engineThread{};
};