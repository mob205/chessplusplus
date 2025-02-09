#pragma once

#include "Game/Point.h"
#include "Piece/PieceEnums.h"

class Game;


namespace Engine
{
	using MovePts = std::pair<Point, Point>;

	MovePts generateMove(Game* game, PieceEnums::Team team);
	
}

