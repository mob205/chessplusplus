#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>

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
	return saveGame(fp);
}


bool GameSerializer::saveGame(std::ostream& out) const
{
	for (const auto& record : game.moveHistory)
	{
		out << record.move->getStart() << record.move->getEnd();
		if (record.result.moveType == MoveResult::Type::Promotion)
		{
			char c{ getPieceAsChar(record.result.promotion.promotionType) };
			out << c;
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
	
	return loadGame(fp);
}

GameSerializer::LoadGameResult GameSerializer::loadGame(std::istream& in) const
{
	Point start{};
	Point end{};

	while (in >> start >> end)
	{
		if (!game.processTurn(start, end) && !game.processTurn(start, end, readPromoType(in)))
		{
			return SaveInvalid;
		}
	}
	return LoadSuccessful;
}

char GameSerializer::readPromoType(std::istream& fp) const
{
	char input{};
	fp >> input;
	return input;
}

void GameSerializer::copyGame(const Game& source) const
{
	std::stringstream buf{};

	source.getSerializer().saveGame(buf);
	loadGame(buf);
}


