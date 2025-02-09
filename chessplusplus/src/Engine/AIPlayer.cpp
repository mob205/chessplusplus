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





namespace Engine
{
	static constexpr std::array<int, PieceEnums::MaxTypes> pieceValues{ 0, 1, 3, 3, 5, 9, 0 };

	constexpr int possessionFactor{ 100 };
	constexpr int mobilityFactor{ 5 };
	constexpr int threatFactor{ 25 };
	constexpr int pawnDevFactor{ 10 };

	constexpr int rookMovePenalty{ 100 };

	constexpr int badPawnStructurePenalty{ 15 };
	constexpr int staggeredPawnBonus{ 10 };

	static constexpr int knightDevelopmentBonus[8][8] = {
		{ -5, -4, -3, -3, -3, -3, -4, -5 },
		{ -4, -2,  0,  0,  0,  0, -2, -4 },
		{ -3,  0,  2,  3,  3,  2,  0, -3 },
		{ -3,  1,  3,  4,  4,  3,  1, -3 },
		{ -3,  0,  3,  4,  4,  3,  0, -3 },
		{ -3,  1,  2,  3,  3,  2,  1, -3 },
		{ -4, -2,  0,  1,  1,  0, -2, -4 },
		{ -5, -4, -3, -3, -3, -3, -4, -5 }
	};

	static int evaluate(const Board& board, PieceEnums::Team team)
	{
		int score{};

		for (int rank = 0; rank < Settings::boardSize; ++rank)
		{
			for (int file = 0; file < Settings::boardSize; ++file)
			{
				Piece* piece = board[{rank, file}].get();
				if (!piece) { continue; }

				Point pos{ rank, file };
				PieceEnums::Type pieceType = piece->getType();
				PieceEnums::Team pieceTeam = piece->getTeam();

				// Favor possessing pieces
				int curScore{};
				curScore += possessionFactor * pieceValues[piece->getType()];

				// Mobility bonus
				MoveSet moves{ piece->getPossibleMoves(board) };
				curScore += mobilityFactor * static_cast<int>(moves.size());

				// Threat bonus
				for (const auto& move : moves)
				{
					if (isEnemyPiece(board, move.first, team))
					{
						curScore += threatFactor * pieceValues[board[move.first]->getType()];
					}
				}

				// Pawn development bonus
				if (pieceType == PieceEnums::Pawn)
				{
					if (pieceTeam == PieceEnums::White)
					{
						curScore += pawnDevFactor * rank - 1;
					}
					else
					{
						curScore += pawnDevFactor * (6 - rank);
					}

					if (isDoubledPawn(board, pos, team) || isIsolatedPawn(board, pos, team))
					{
						curScore -= badPawnStructurePenalty;
					}
					if (isStaggeredPawn(board, pos, team))
					{
						curScore += staggeredPawnBonus;
					}
				}

				// Knight development bonus
				if (pieceType == PieceEnums::Knight)
				{
					curScore += knightDevelopmentBonus[rank][file];
				}

				if (pieceType == PieceEnums::Rook)
				{
					if (piece->getMoved())
					{
						curScore -= rookMovePenalty;
					}
				}

				if (pieceTeam == team)
				{
					score += curScore;
				}
				else
				{
					score -= curScore;
				}

			}
		}
		return score;
	}

	static int isDoubledPawn(const Board& board, const Point& pos, PieceEnums::Team team)
	{
		Point left{ pos.rank - 1, pos.file };
		Point right{ pos.rank + 1, pos.file };
		return (left.isInBounds() && board[left] && board[left]->getType() == PieceEnums::Pawn)
			|| (right.isInBounds() && board[right] && board[right]->getType() == PieceEnums::Pawn);
	}

	static int isIsolatedPawn(const Board& board, const Point& pos, PieceEnums::Team team)
	{
		for (int dRank = -1; dRank <= 1; ++dRank)
		{
			for (int dFile = -1; dFile <= 1; ++dFile)
			{
				Point curPos{ pos.rank + dRank, pos.file + dFile };
				if (curPos.isInBounds() && board[curPos])
				{
					return false;
				}
			}
		}
		return true;
	}

	static int isStaggeredPawn(const Board& board, const Point& pos, PieceEnums::Team team)
	{
		Point botleft{ pos.rank - 1, pos.file - 1 };
		Point botright{ pos.rank - 1, pos.file + 1 };
		Point topleft{ pos.rank + 1, pos.file - 1 };
		Point topright{ pos.rank + 1, pos.file + 1 };

		return botleft.isInBounds() && board[botleft] && board[botleft]->getType() == PieceEnums::Pawn
			|| botright.isInBounds() && board[botright] && board[botright]->getType() == PieceEnums::Pawn
			|| topleft.isInBounds() && board[topleft] && board[topleft]->getType() == PieceEnums::Pawn
			|| topright.isInBounds() && board[topright] && board[topright]->getType() == PieceEnums::Pawn;
	}

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
		alphaBeta(game, team, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 3, bestMove);
		return bestMove;
	}
	
	static int alphaBeta(Game* game, PieceEnums::Team team, int alpha, int beta, int depth, MovePts& outBestMove)
	{
		if (depth == 0) { return evaluate(game->getBoard(), team); }
		int bestValue = std::numeric_limits<int>::min();
		
		auto moves = getValidMoves(game, team);
		for (const auto& move : moves)
		{
			MoveResult res = game->processTurn(move.first, move.second, 'Q');

			int score{};
			if (res.oppStatus == MoveResult::OpponentStatus::Checkmate)
			{
				score = std::numeric_limits<int>::max();
			}
			else
			{
				MovePts tempBestMove{};
				score = -alphaBeta(game, getOppositeTeam(team), -beta, -alpha, depth - 1, tempBestMove);
			}
			game->undoMove();
			if (score > bestValue)
			{
				outBestMove = move;
				bestValue = score;
				if (score > alpha)
				{
					alpha = score;
				}
			}
			if (score >= beta)
			{
				return bestValue;
			}
		}
		return bestValue;
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


