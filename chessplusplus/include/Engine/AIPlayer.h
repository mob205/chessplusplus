#pragma once

#include "Game/Point.h"
#include "Piece/PieceEnums.h"

class Game;

static void addValidatedMoves(const Board& board, const Point& pos, Game* game, std::vector<std::pair<Point, Point>>& moves);
static std::vector<std::pair<Point, Point>> getValidMoves(Game* game, PieceEnums::Team team);

namespace Engine
{
	std::pair<Point, Point> generateMove(Game* game, PieceEnums::Team team);
}

static void NewFunction(const Board& board, const Point& pos, Game* game, std::vector<std::pair<Point, Point>>& moves);
