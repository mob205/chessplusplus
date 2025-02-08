#pragma once

#include "Game/Point.h"
#include "Piece/PieceEnums.h"

class Game;

class AIPlayer
{
public:
	AIPlayer(Game& game, PieceEnums::Team team)
		: game{ game }, team{ team }
	{}

	std::pair<Point, Point> generateMove();

private:

	Game& game;
	PieceEnums::Team team;

	std::vector<std::pair<Point, Point>> getValidMoves();
};