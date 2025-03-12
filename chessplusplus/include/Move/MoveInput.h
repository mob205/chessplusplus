#pragma once
#include "Game/Point.h"
#include "Piece/PieceEnums.h"

struct MoveInput
{
	MoveInput() = default;
	MoveInput(Point start, Point end)
		: start{ start }, end{ end }
	{}

	MoveInput(std::pair<Point, Point> pair)
		: start{ pair.first }, end{ pair.second }
	{}

	MoveInput(Point start, Point end, PieceEnums::Type type)
		: start{ start }, end{ end }, extraInput{ PieceEnums::pieceSymbols[type] }
	{}

	Point start{};
	Point end{};
	char extraInput{ 'Q' };
};