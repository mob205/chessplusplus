#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <iostream>
#include "Board/Board.h"
#include "Board/BoardHelpers.h"
#include "Game/Settings.h"
#include "Game/Game.h"
#include "Engine/AIPlayer.h"


static constexpr std::array<int, PieceEnums::MaxTypes> pieceValues{ 0, 1, 3, 3, 5, 9, 0 };



namespace Engine
{
	MovePts getRandomMove(Game* game, PieceEnums::Team team)
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

	MovePts generateMove(Game* game, PieceEnums::Team team)
	{
		srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

		MovePts bestMove{};
		maxi(game, team, 2, bestMove);
		return bestMove;
	}

	static int evaluate(const Board& board, PieceEnums::Team team)
	{
		int score{};

		for (int rank = 0; rank < Settings::boardSize; ++rank)
		{
			for (int file = 0; file < Settings::boardSize; ++file)
			{
				Piece* piece = board[{rank, file}].get();
				if (piece)
				{
					if (piece->getTeam() == team)
					{
						score += pieceValues[piece->getType()];
					}
					else
					{
						score -= pieceValues[piece->getType()];
					}
				}
			}
		}
		return score;
	}

	static int maxi(Game* game, PieceEnums::Team team, int depth, MovePts& outBestMove)
	{
		if (depth == 0) { return evaluate(game->getBoard(), team); }
		int max = std::numeric_limits<int>::min();

		auto moves = getValidMoves(game, team);
		for (const auto& move : moves)
		{
			MoveResult res = game->processTurn(move.first, move.second, 'Q');

			int score{};

			if (res.oppStatus == MoveResult::OpponentStatus::Checkmate)
			{
				score = std::numeric_limits<int>::max();
			}
			else if (res.oppStatus == MoveResult::OpponentStatus::Stalemate)
			{
				score = 0;
			}
			else
			{
				MovePts move{};
				score = mini(game, getOppositeTeam(team), depth - 1, move);
			}

			if (score > max)
			{
				max = score;
				outBestMove = move;
			}
			game->undoMove();
		}

		return max;
	}

	static int mini(Game* game, PieceEnums::Team team, int depth, MovePts& outBestMove)
	{
		if (depth == 0) { return -evaluate(game->getBoard(), team); }
		int min = std::numeric_limits<int>::max();

		auto moves = getValidMoves(game, team);
		for (const auto& move : moves)
		{
			MoveResult res = game->processTurn(move.first, move.second, 'Q');

			int score{};

			if (res.oppStatus == MoveResult::OpponentStatus::Checkmate)
			{
				score = std::numeric_limits<int>::min();
			}
			else if (res.oppStatus == MoveResult::OpponentStatus::Stalemate)
			{
				score = 0;
			}
			else
			{
				MovePts move{};
				score = maxi(game, getOppositeTeam(team), depth - 1, move);
			}

			if (score > min)
			{
				min = score;
				outBestMove = move;
			}
			game->undoMove();
		}
		return min;
	}
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


	
}


