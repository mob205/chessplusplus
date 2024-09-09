#include <iostream>
#include <fstream>
#include <functional>

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
		if (move->getExtraInput())
		{
			fp << move->getExtraInput();
		}
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

	auto extraInput{ [this, &fp]() {
		return this->readPromoType(fp);
	}};

	while (fp >> start >> end)
	{
		if (!game.processTurn(start, end, false, extraInput))
		{
			return false;
		}
	}
	return true;
}

char GameSerializer::readPromoType(std::ifstream& fp) const
{
	char input{};
	fp >> input;
	return input;
}