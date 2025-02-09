#include <vector>
#include <random>
#include <chrono>
#include "Board/Board.h"
#include "Board/BoardHelpers.h"
#include "Game/Settings.h"
#include "Game/Game.h"
#include "Engine/AIPlayer.h"



static void addValidatedMoves(const Board& board, const Point& pos, Game* game, std::vector<std::pair<Point, Point>>& moves)
{
	MoveSet unvalidatedMoves = board[pos]->getPossibleMoves(board);
	for (const auto& move : unvalidatedMoves)
	{
		Point start = move.second->getStart();
		Point end = move.first;
		if (game->isValidMove(start, end))
		{
			moves.push_back({ start, end });
		}
	}
}

static std::vector<std::pair<Point, Point>> getValidMoves(Game* game, PieceEnums::Team team)
{
	std::vector<std::pair<Point, Point>> moves{};

	const Board& board = game->getBoard();

	for (int rank = 0; rank < Settings::boardSize; ++rank)
	{
		for (int file = 0; file < Settings::boardSize; ++file)
		{
			Point pos{ rank, file };
			if (isAlliedPiece(board, pos, team))
			{
				addValidatedMoves(board, pos, game, moves);
			}
		}
	}
	return moves;
}



namespace Engine
{

	std::pair<Point, Point> generateMove(Game* game, PieceEnums::Team team)
	{
		auto moves = getValidMoves(game, team);

		srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

		if (moves.size() == 0)
		{
			return { {0, 0}, {0, 0} };
		}
		int idx = std::rand() % (moves.size());
		return moves[idx];
	}
}


