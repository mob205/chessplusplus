#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <fstream>

#include "Board/Board.h"
#include "Board/BoardHelpers.h"
#include "Game/Settings.h"
#include "Game/Game.h"
#include "Engine/AIPlayer.h"

constexpr bool debugShowThinking{ false };
std::ofstream out;

namespace Engine
{
	static int evaluate(const Board& board, PieceEnums::Team team);
	static int searchMoves(Game* game, PieceEnums::Team team, int alpha, int beta, int depth, MovePts& outBestMove);

	static void addValidatedMoves(const Board& board, const Point& pos, Game* game, std::vector<std::pair<MovePts, int>>& moves);

	// Returns list of the endpoints of all validated moves, along with a heuristic score of the move for sorting
	static std::vector<std::pair<MovePts, int>> getValidMoves(Game* game, PieceEnums::Team team);

	static int isDoubledPawn(const Board& board, const Point& pos, PieceEnums::Team team);
	static int isIsolatedPawn(const Board& board, const Point& pos, PieceEnums::Team team);
	static int isStaggeredPawn(const Board& board, const Point& pos, PieceEnums::Team team);
}

static void printDepthPrefix(int depth)
{
	for (int i = 0; i < 3 - depth; ++i)
	{
		out << '|';
	}
	out << '-';
}

static void printThinkingMessage(int depth, const std::string& str)
{
	printDepthPrefix(depth);
	out << str;
}

namespace Engine
{

	/*
	*	Board evaluation
	*/

#pragma region evaluation
	constexpr std::array<int, PieceEnums::MaxTypes> pieceValues{ 0, 1, 3, 3, 5, 9, 0 };

	constexpr int possessionFactor{ 25 };
	constexpr int mobilityFactor{ 1 };
	constexpr int threatFactor{ 3 };
	constexpr int pawnDevFactor{ 5 };

	constexpr int badPawnStructurePenalty{ 5 };
	constexpr int staggeredPawnBonus{ 7 };

	static constexpr int centerBonusBoard[8][8] = {
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
					if (isEnemyPiece(board, move.first, pieceTeam))
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

					if (isDoubledPawn(board, pos, pieceTeam) || isIsolatedPawn(board, pos, pieceTeam))
					{
						curScore -= badPawnStructurePenalty;
					}
					if (isStaggeredPawn(board, pos, pieceTeam))
					{
						curScore += staggeredPawnBonus;
					}
				}

				// Development bonus
				if (pieceType == PieceEnums::Knight || pieceType == PieceEnums::Bishop || pieceType == PieceEnums::Pawn)
				{
					curScore += centerBonusBoard[rank][file];
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
		Point left{ pos.rank, pos.file - 1 };
		Point right{ pos.rank, pos.file + 1 };
		return (left.isInBounds() && board[left] && board[left]->getTeam() == team && board[left]->getType() == PieceEnums::Pawn && static_cast<Pawn*>(board[left].get())->getMoved())
			|| (right.isInBounds() && board[right] && board[right]->getTeam() == team && board[right]->getType() == PieceEnums::Pawn && static_cast<Pawn*>(board[right].get())->getMoved());
	}

	static int isIsolatedPawn(const Board& board, const Point& pos, PieceEnums::Team team)
	{
		for (int dRank = -1; dRank <= 1; ++dRank)
		{
			for (int dFile = -1; dFile <= 1; ++dFile)
			{
				Point curPos{ pos.rank + dRank, pos.file + dFile };
				if (curPos.isInBounds() && isAlliedPiece(board, curPos, team))
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
#pragma endregion


	/*
	*	Move searching
	*/
#pragma region searching
	MovePts getRandomMove(Game* game, PieceEnums::Team team)
	{
		auto moves = getValidMoves(game, team);

		srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

		if (moves.size() == 0)
		{
			return { {0, 0}, {0, 0} };
		}
		int idx = std::rand() % (moves.size());
		return moves[idx].first;
	}

	MovePts generateMove(Game* game, PieceEnums::Team team)
	{
		if constexpr (debugShowThinking)
		{
			out.open("thinklog.txt", std::ofstream::trunc);
		}

		MovePts bestMove{};
		searchMoves(game, team, -99999, 99999, 3, bestMove);

		if constexpr (debugShowThinking)
		{
			out.close();
		}

		return bestMove;
	}
	
	static int searchMoves(Game* game, PieceEnums::Team team, int alpha, int beta, int depth, MovePts& outBestMove)
	{
		if constexpr (debugShowThinking) { printThinkingMessage(depth, std::format("Depth: {} | Alpha: {} | Beta: {} \n", depth, alpha, beta)); }

		if (depth == 0)
		{
			if constexpr (debugShowThinking)
			{
				int eval = evaluate(game->getBoard(), team);
				printThinkingMessage(depth, std::format("Returning {} evaluation for {}\n", eval, (team == PieceEnums::White ? "White" : "Black")));
				printDepthPrefix(depth);
				return eval;
			}
			else
			{
				return evaluate(game->getBoard(), team);
			}
		}

		int bestValue = std::numeric_limits<int>::min();
		
		auto moves = getValidMoves(game, team);

		std::sort(moves.begin(), moves.end(),
			[](std::pair<MovePts, int> a, std::pair<MovePts, int> b)
			{
				return b.second < a.second;
			}
		);

		for (const auto& move : moves)
		{
			if constexpr (debugShowThinking) { printThinkingMessage(depth, std::format("Considering {} to {}.\n", move.first.first, move.first.second)); }

			MoveResult res = game->processTurn(move.first.first, move.first.second, 'Q');

			int score{};
			if (res.oppStatus == MoveResult::OpponentStatus::Checkmate)
			{
				score = std::numeric_limits<int>::max();
			}
			else
			{
				MovePts tempBestMove{};
				score = -searchMoves(game, getOppositeTeam(team), -beta, -alpha, depth - 1, tempBestMove);

				if constexpr (debugShowThinking) { printThinkingMessage(depth, std::format("Branch {} to {} yielded {}.\n", move.first.first, move.first.second, score)); }
			}
			game->undoMove();
			if (score > bestValue)
			{
				outBestMove = move.first;
				bestValue = score;
				if (score > alpha)
				{
					alpha = score;
					if (alpha >= beta)
					{
						if constexpr (debugShowThinking) { printThinkingMessage(depth, std::format("Cut off triggered. Choosing {} to {} at {}.\n", outBestMove.first, outBestMove.second, bestValue)); }

						return bestValue;
					}
				}
			}
			
		}
		if constexpr (debugShowThinking) { printThinkingMessage(depth, std::format("Exhausted all moves. Choosing {} to {} at {}.\n", outBestMove.first, outBestMove.second, bestValue)); }

		return bestValue;
	}

#pragma endregion

	/*
	*	Valid move generation/ranking
	*/

#pragma region move generation

	constexpr int capturePieceValueFactor{ 45 };
	constexpr int checkmateBonus{ 9999999 };

	constexpr int castleBonus{ 250 };
	constexpr int doublePawnBonus{ 10 };
	constexpr int enpassantBonus{ 1000 };
	constexpr int promotionBonus{ 500 };

	constexpr int centerBonusFactor{ 10 };

	static int analyzeMove(const MoveResult& move)
	{
		int score{};
		if (move.oppStatus == MoveResult::OpponentStatus::Checkmate)
		{
			score += checkmateBonus;
		}
		if (move.oppStatus == MoveResult::OpponentStatus::Stalemate)
		{
			score -= checkmateBonus;
		}

		score += capturePieceValueFactor * pieceValues[move.capturedPieceType];

		switch (move.moveType)
		{
		case MoveResult::Type::Castle:
			score += castleBonus;
			break;

		case MoveResult::Type::DoublePawn:
			score += doublePawnBonus;
			break;

		case MoveResult::Type::EnPassant:
			score += enpassantBonus;
			break;

		case MoveResult::Type::Promotion:
			score += promotionBonus;
			break;
		}

		score += centerBonusFactor * centerBonusBoard[move.end.rank][move.end.file];

		if (move.movedPieceType == PieceEnums::Rook)
		{
			score -= checkmateBonus;
		}

		return score;
	}

	static void addValidatedMoves(const Board& board, const Point& pos, Game* game, std::vector<std::pair<MovePts, int>>& moves)
	{
		MoveSet unvalidatedMoves = board[pos]->getPossibleMoves(board);
		MoveResult res{};
		for (const auto& move : unvalidatedMoves)
		{
			Point start = move.second->getStart();
			Point end = move.first;
			if (game->isValidMove(start, end, &res))
			{
				moves.push_back({{start, end}, analyzeMove(res)});
			}
		}
	}

	static std::vector<std::pair<MovePts, int>> getValidMoves(Game* game, PieceEnums::Team team)
	{
		std::vector<std::pair<MovePts, int>> moves{};

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

#pragma endregion
}


