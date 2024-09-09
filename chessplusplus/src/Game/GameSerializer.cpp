#include <iostream>
#include <fstream>

#include "Game/GameSerializer.h"
#include "Game/Game.h"

bool GameSerializer::saveGame(const std::string& savename) const
{
	std::ofstream fp{ saveDirectory + savename + extension };

	if (!fp)
	{
		return false;
	}
	for (const auto& move : game.moveHistory)
	{
		fp << move->getStart() << move->getEnd();
	}
	return true;
}

bool GameSerializer::loadGame(const std::string& savename) const
{
	std::ifstream fp{ saveDirectory + savename + extension };
	if (!fp)
	{
		return false;
	}
	Point start{};
	Point end{};
	while (fp >> start >> end)
	{
		if (!game.processTurn(start, end, false))
		{
			return false;
		}
	}
	return true;
}