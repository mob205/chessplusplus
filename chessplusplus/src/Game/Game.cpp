#include "Game/Game.h"
#include "Game/Settings.h"
#include "Input/Input.h"
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
	for (int i = 0; i < Settings::g_boardSize; ++i)
	{
		Point blackPos{ Settings::g_boardSize - 2, i };
		Point whitePos{ 1, i };

		board[whitePos] = std::make_unique<Pawn>(whitePos, PieceEnums::White, currentTurn);
		board[blackPos] = std::make_unique<Pawn>(blackPos, PieceEnums::Black, currentTurn);
	}

	// Setup pieces
	for (int i = 0; i < PieceEnums::MaxTeams; ++i)
	{
		// 0 for white, 7 for black
		int rank{ i * (Settings::g_boardSize - 1) };
		PieceEnums::Team team{ static_cast<PieceEnums::Team>(i) };
		PieceEnums::Team opponent{ static_cast<PieceEnums::Team>((i + 1) % PieceEnums::MaxTeams) };

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

bool Game::isInCheck(PieceEnums::Team team)
{
	PieceEnums::Team oppTeam{ static_cast<PieceEnums::Team>((currentTeam + 1) % PieceEnums::MaxTeams) };
	return attackBoard.isAttacking(kings[currentTeam]->getPosition(), oppTeam);
}

void Game::playGame()
{
	Board& board{ getBoard() };
	InputResult start{};
	while (start.result != InputResult::QUIT)
	{
		static std::unique_ptr<Move> savedMove{};

		currentTeam = static_cast<PieceEnums::Team>((currentTurn) % PieceEnums::MaxTeams);
		PieceEnums::Team oppTeam{ static_cast<PieceEnums::Team>((currentTeam + 1) % PieceEnums::MaxTeams) };
		
		attackBoard.update(board, currentTeam, kings);

		std::cout << '\n' << board << '\n';

		// Check for end conditions
		
	
		std::cout << (currentTeam ? "Black" : "White") << "'s turn!\n";
		std::cout << "Select a piece to move, or type 'QUIT' to quit.\n";


		start = Input::getTileInput();
		if (start.result == InputResult::QUIT)
		{
			break;
		}

		else if (start.result == InputResult::SAVE)
		{
			if (serializer.saveGame(Input::getSaveName()))
			{
				std::cout << "Successfully saved the game.\n";
			}
			else
			{
				std::cout << "Failed to save the game.\n";
			}
			continue;
		}
		else if (start.result == InputResult::UNDO)
		{
			if (!moveHistory.empty())
			{
				std::unique_ptr<Move> move = std::move(moveHistory.back().move);
				moveHistory.pop_back();
				move->undoMove(board);
				std::cout << "Undo successful.\n";
				--currentTurn;
			}
			else
			{
				std::cout << "No move to undo!\n";
			}
			continue;
		}

		// Must be point result
		if (!ownsPiece(board, start.point, currentTeam))
		{
			std::cout << "No owned piece is on this square!\n";
			continue;
		}
		

		std::cout << "Selected a " << board[start.point]->getName() << "\n\n";


		std::cout << "Select a tile to move to, or type 'QUIT' to unselect the piece.\n";
		InputResult end = Input::getTileInput();
		
		if (end.result == InputResult::QUIT)
		{
			continue;
		}
		if (end.result != InputResult::POINT)
		{
			std::cout << "Invalid input.\n";
			continue;
		}

		MoveResult res{ processTurn(start.point, end.point, true, &Input::getPromotionType) };
		
		if (!res)
		{
			switch (res.reasonFailed)
			{
			case MoveResult::MoveFailReason::InvalidPiece:
				std::cout << "Invalid move. There is no owned piece on this tile.\n";
				break;
			case MoveResult::MoveFailReason::Check:
				std::cout << "Invalid move. King in check must be defended.\n";
				break;
			case MoveResult::MoveFailReason::Pinned:
				std::cout << "Invalid move. Unable to take move that would put king in check";
				break;
			case MoveResult::MoveFailReason::NotInSet:
				std::cout << "Invalid move. Piece is unable to move in this way.";
				break;
			}
			continue;
		}
		
		// Valid move
		switch (res.type)
		{
		case MoveResult::Type::Standard:
			if (res.standard.capturedPiece) 
			{
				std::cout << "Captured an enemy " << PieceEnums::pieceNames[res.standard.capturedPiece] << '\n';
			}
			break;
		case MoveResult::Type::Castle:
			std::cout << "Castled!";
			break;
		case MoveResult::Type::EnPassant:
			std::cout << "EN PASSANT!!!!";
			break;
		case MoveResult::Type::Promotion:
			if (res.promotion.capturedPiece)
			{
				std::cout << "Captured an enemy " << PieceEnums::pieceNames[res.promotion.capturedPiece] << '\n';
			}
			std::cout << "Promoted pawn to " << PieceEnums::pieceNames[res.promotion.promotionType] << '\n';
			break;
		}

		switch (res.oppStatus)
		{
		case MoveResult::OpponentStatus::Check:
			std::cout << "Check!\n";
			break;
		case MoveResult::OpponentStatus::Checkmate:
			std::cout << "CHECKMATE! " << (currentTeam ? "Black" : "White") << " WINS!!\n";
			return;
		case MoveResult::OpponentStatus::Stalemate:
			std::cout << "Stalemate.\n";
			return;
		}
	}
}

// Processes a turn given valid start and end inputs
MoveResult Game::processTurn(const Point& start, const Point& end, bool printMove, std::function<char()> getExtraInput)
{
	MoveResult failResult{};

	if (!board[start])
	{
		failResult.reasonFailed = MoveResult::MoveFailReason::InvalidPiece;
		return failResult;
	}

	// Check if the input is a valid move of the selected piece
	MoveSet possibleMoves{ board[start]->getPossibleMoves(board) };
	auto moveItr{ possibleMoves.find(end) };

	// Not a valid move
	if (moveItr == possibleMoves.end())
	{
		failResult.reasonFailed = MoveResult::MoveFailReason::NotInSet;
		return failResult;
	}

	// Check if the king is in check before the move to distinguish potential fail case
	attackBoard.update(board, currentTeam, kings);
	bool isInitialCheck{ isInCheck(currentTeam) };


	// Check if the moved piece was pinned
	moveItr->second->executeMove(board);
	attackBoard.update(board, currentTeam, kings);
	moveItr->second->undoMove(board);
	if (isInCheck(currentTeam))
	{
		// Cannot end turn with king in check
		failResult.reasonFailed = isInitialCheck ? MoveResult::MoveFailReason::Check : MoveResult::MoveFailReason::Pinned;
		return failResult;
	}

	// Complete move
	MoveResult moveResult{ moveItr->second->executeMove(board, getExtraInput) };

	// Go to next turn
	moveHistory.emplace_back(std::move(moveItr->second), moveResult);
	++currentTurn;

	PieceEnums::Team opp{ getOppositeTeam(currentTeam) };
	attackBoard.update(board, opp, kings);

	// Check end conditions
	if (kings[opp]->getPossibleMoves(board).size() == 0 && !hasPossibleNonKingMove(opp))
	{
		moveResult.oppStatus = isInCheck(opp) ? MoveResult::OpponentStatus::Checkmate : MoveResult::OpponentStatus::Stalemate;
	}
	else if (isInCheck(currentTeam))
	{
		moveResult.oppStatus = MoveResult::OpponentStatus::Check;
	}
	return moveResult;
}

// Returns true if the specified player has a valid piece move on the current board
bool Game::hasPossibleNonKingMove(PieceEnums::Team team)
{
	PieceEnums::Team opp{ getOppositeTeam(currentTeam)};
	AttackBoard attackBoard{};

	for (int rank = 0; rank < Settings::g_boardSize; ++rank)
	{
		for (int file = 0; file < Settings::g_boardSize; ++file)
		{
			Point pos{ rank, file };
			if (!isAlliedPiece(board, pos, team) || board[pos]->getType() == PieceEnums::King) { continue; }
			
			MoveSet set{ board[pos]->getPossibleMoves(board) };

			// Simulate a move to ensure the piece isn't pinned
			for (auto& move : set)
			{
				move.second->executeMove(board);
				attackBoard.update(board, opp, kings);
				move.second->undoMove(board);

				// Valid move found!
				if (!isInCheck(team))
				{
					return true;
				}
			}
		}
	}
	// Checked every move and none are valid
	return false;
}

PieceEnums::Team getOppositeTeam(PieceEnums::Team team)
{
	return static_cast<PieceEnums::Team>((team + 1) % PieceEnums::MaxTeams);
}