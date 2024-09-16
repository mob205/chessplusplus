#include "Game/Game.h"
#include "Game/Settings.h"
#include "ConsoleInput/Input.h"
#include "Board/BoardHelpers.h"

#include "Piece/PieceEnums.h"
#include "Piece/Pawn.h"
#include "Piece/Knight.h"
#include "Piece/Bishop.h"
#include "Piece/Rook.h"
#include "Piece/Queen.h"
#include "Piece/King.h"

template<typename T>
static void addPiece(Board& board, const Point& position, PieceEnums::Team team)
{
	board[position] = std::make_unique<T>(position, team);
}

Game::Game()
{
	// Setup pawns
	for (int i = 0; i < Settings::boardSize; ++i)
	{
		Point blackPos{ Settings::boardSize - 2, i };
		Point whitePos{ 1, i };

		board[whitePos] = std::make_unique<Pawn>(whitePos, PieceEnums::White, currentTurn);
		board[blackPos] = std::make_unique<Pawn>(blackPos, PieceEnums::Black, currentTurn);
	}

	// Setup pieces
	for (int i = 0; i < PieceEnums::MaxTeams; ++i)
	{
		// 0 for white, 7 for black
		int rank{ i * (Settings::boardSize - 1) };
		PieceEnums::Team team{ static_cast<PieceEnums::Team>(i) };

		addPiece<Rook>(board, { rank, 0 }, team);
		addPiece<Knight>(board, { rank, 1 }, team);
		addPiece<Bishop>(board, { rank, 2 }, team);
		addPiece<Queen>(board, { rank, 3 }, team);

		// Create a new king
		auto king{ std::make_unique<King>(Point{ rank, 4 }, team, attackBoard) };
		// Save a non-owning raw pointer to view the king later
		kings[i] = king.get();
		// Move it onto the board
		board[{rank, 4}] = std::move(king);


		addPiece<Bishop>(board, { rank, 5 }, team);
		addPiece<Knight>(board, { rank, 6 }, team);
		addPiece<Rook>(board, { rank, 7 }, team);
	}
}


MoveResult Game::processTurn(const Point& start, const Point& end, char extraInput)
{
	PieceEnums::Team curTeam{ getCurrentTeam() };
	MoveResult failResult{};

	if (!board[start] || board[start]->getTeam() != curTeam)
	{
		failResult.reasonFailed = MoveResult::MoveFailReason::InvalidPiece;
		return failResult;
	}

	attackBoard.update(board, curTeam, kings);

	// Check if the king is in check before the move to distinguish pinning from being in check
	bool isInitialCheck{ isInCheck(curTeam, attackBoard) };

	// Check if the input is a valid move of the selected piece
	MoveSet possibleMoves{ board[start]->getPossibleMoves(board) };
	auto moveItr{ possibleMoves.find(end) };

	// Not a valid move
	if (moveItr == possibleMoves.end())
	{
		failResult.reasonFailed = MoveResult::MoveFailReason::NotInSet;
		return failResult;
	}

	// Check if move would put king in check
	MoveResult moveResult{ moveItr->second->executeMove(board, extraInput) };
	attackBoard.update(board, curTeam, kings);
	if (isInCheck(curTeam, attackBoard))
	{
		// Cannot end turn with king in check
		moveItr->second->undoMove(board);
		failResult.reasonFailed = isInitialCheck ? MoveResult::MoveFailReason::Check : MoveResult::MoveFailReason::Pinned;
		return failResult;
	}
	else if (moveResult.reasonFailed == MoveResult::MoveFailReason::NeedsInput)
	{
		moveItr->second->undoMove(board);
		return moveResult;
	}

	moveResult.oppStatus = checkEndConditions();

	// Go to next turn
	moveHistory.emplace_back(std::move(moveItr->second), moveResult);
	++currentTurn;
	
	return moveResult;
}

MoveResult::OpponentStatus Game::checkEndConditions()
{
	PieceEnums::Team cur{ getCurrentTeam() };
	PieceEnums::Team opp{ getOppositeTeam(cur) };
	attackBoard.update(board, opp, kings);

	// Check end conditions
	if (kings[opp]->getPossibleMoves(board).size() == 0 && !hasPossibleNonKingMove(opp))
	{
		return isInCheck(opp) ? MoveResult::OpponentStatus::Checkmate : MoveResult::OpponentStatus::Stalemate;
	}
	else if (isInCheck(opp))
	{
		return MoveResult::OpponentStatus::Check;
	}
	return MoveResult::OpponentStatus::Normal;
}

bool Game::isInCheck(PieceEnums::Team team) const
{
	PieceEnums::Team oppTeam{ getOppositeTeam(team) };
	return attackBoard.isAttacking(kings[team]->getPosition(), oppTeam);
}

bool Game::isInCheck(PieceEnums::Team team, const AttackBoard& curAttackBoard) const
{
	PieceEnums::Team oppTeam{ getOppositeTeam(team) };
	return curAttackBoard.isAttacking(kings[team]->getPosition(), oppTeam);
}

bool Game::undoMove()
{
	if (!moveHistory.empty())
	{
		std::unique_ptr<Move> move{ std::move(moveHistory.back().move) };
		moveHistory.pop_back();
		move->undoMove(board);
		--currentTurn;
		return true;
	}
	return false;
}


// Returns true if the specified player has a valid piece move on the current board
bool Game::hasPossibleNonKingMove(PieceEnums::Team team)
{
	PieceEnums::Team opp{ getOppositeTeam(team)};
	AttackBoard tempAttackBoard{};

	for (int rank = 0; rank < Settings::boardSize; ++rank)
	{
		for (int file = 0; file < Settings::boardSize; ++file)
		{
			Point pos{ rank, file };
			if (!isAlliedPiece(board, pos, team) || board[pos]->getType() == PieceEnums::King) { continue; }
			
			MoveSet set{ board[pos]->getPossibleMoves(board) };

			// Simulate a move to ensure doesn't leave king in check
			for (auto& move : set)
			{
				move.second->executeMove(board);
				tempAttackBoard.update(board, opp, kings);
				move.second->undoMove(board);

				// Valid move found!
				if (!isInCheck(team, tempAttackBoard))
				{
					return true;
				}
			}
		}
	}
	// Checked every move and none are valid
	return false;
}