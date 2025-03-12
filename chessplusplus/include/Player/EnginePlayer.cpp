#include "EnginePlayer.h"
#include "Engine/Engine.h"
#include <future>
#include <thread>

void EnginePlayer::startTurn(const Game& game)
{

	engineThread = std::async(std::launch::async,
		[this, &game]
		{
			MoveInput move = Engine::generateMove(game, team);
			return move;
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
