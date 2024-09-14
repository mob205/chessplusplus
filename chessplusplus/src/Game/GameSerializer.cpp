#include <iostream>
#include <fstream>
#include <functional>

#include "Game/GameSerializer.h"
#include "Game/Game.h"
#include "Move/Move.h"
#include "Move/MoveResult.h"
#include "Piece/PieceEnums.h"

static char getPieceAsChar(PieceEnums::Type type)
{
	switch (type)
	{
	case PieceEnums::Queen:
		return 'Q';
	case PieceEnums::Bishop:
		return 'B';
	case PieceEnums::King:
		return 'K';
	case PieceEnums::Knight:
		return 'N';
	default:
		// Since this is only used for promotion right now,
		// no need to check for other types
		return 'Q';
	}
}

bool GameSerializer::saveGame(const std::string& savename) const
{
	std::ofstream fp{ saveDirectory + savename + extension };

	if (!fp)
	{
		return false;
	}
	for (const auto& record : game.moveHistory)
	{
		fp << record.move->getStart() << record.move->getEnd();
		if (record.result.type == MoveResult::Type::Promotion)
		{
			char c{ getPieceAsChar(record.result.promotion.promotionType) };
			fp << c;
		}
	}
	return true;
}

GameSerializer::LoadGameResult GameSerializer::loadGame(const std::string& savename) const
{
	std::ifstream fp{ saveDirectory + savename + extension };
	if (!fp)
	{
		return SaveNotFound;
	}
	Point start{};
	Point end{};

	auto extraInput{ [this, &fp]() {
		return this->readPromoType(fp);
	}};

	while (fp >> start >> end)
	{
		if (!game.processTurn(start, end, extraInput))
		{
			return SaveInvalid;
		}
	}
	return LoadSuccessful;
}

char GameSerializer::readPromoType(std::ifstream& fp) const
{
	char input{};
	fp >> input;
	return input;
}

