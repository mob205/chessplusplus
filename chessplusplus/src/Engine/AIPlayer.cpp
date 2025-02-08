#include <vector>
#include <random>
#include <chrono>

#include "Board/Board.h"
#include "Board/BoardHelpers.h"
#include "Game/Settings.h"
#include "Game/Game.h"
#include "Engine/AIPlayer.h"

std::pair<Point, Point> AIPlayer::generateMove()
{
	auto moves = getValidMoves();

	srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	int idx = std::rand() % (moves.size() - 1);
	return moves[idx];
}

std::vector<std::pair<Point, Point>> AIPlayer::getValidMoves()
{
	std::vector<std::pair<Point, Point>> moves{};

	const Board& board = game.getBoard();

	for (int rank = 0; rank < Settings::boardSize; ++rank)
	{
		for (int file = 0; file < Settings::boardSize; ++file)
		{
			Point pos{ rank, file };
			if (isAlliedPiece(board, pos, team))
			{
				MoveSet unvalidatedMoves = board[pos]->getPossibleMoves(board);
				for (const auto& move : unvalidatedMoves)
				{
					Point start = move.second->getStart();
					Point end = move.first;
					if (game.isValidMove(start, end))
					{
						moves.push_back({ start, end });
					}
				}
			}
		}
	}
}
