#include "EnginePlayer.h"
#include "Engine/Engine.h"
#include "Game/Game.h"

void EnginePlayer::startTurn(Game* game)
{
	engineThread = std::async(std::launch::async,
		[=]
		{
			return Engine::generateMove(game, team);
		}
	);
}

bool EnginePlayer::getMove(MoveInput& outMove)
{
	using namespace std::chrono_literals;

	std::future_status status = engineThread.wait_for(0ms);

	if (status == std::future_status::ready)
	{
		outMove = engineThread.get();
		return true;
	}
	return false;
}
