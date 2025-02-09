#pragma once

#include "Game/Point.h"
#include "Piece/PieceEnums.h"

class Game;


namespace Engine
{
	using MovePts = std::pair<Point, Point>;

	MovePts generateMove(Game* game, PieceEnums::Team team);
	
	static int evaluate(const Board& board, PieceEnums::Team team);
	static int maxi(Game* game, PieceEnums::Team team, int depth, MovePts& outBestMove);
	static int mini(Game* game, PieceEnums::Team team, int depth, MovePts& outBestMove);

	static void addValidatedMoves(const Board& board, const Point& pos, Game* game, std::vector<MovePts>& moves);
	static std::vector<MovePts> getValidMoves(Game* game, PieceEnums::Team team);
	
}

