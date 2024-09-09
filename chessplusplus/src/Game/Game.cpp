#include "Game/Game.h"
#include "Game/Settings.h"
#include "Input/Input.h"
#include "Board/BoardHelpers.h"

#include "Piece/Pawn.h"
#include "Piece/Knight.h"
#include "Piece/Bishop.h"
#include "Piece/Rook.h"
#include "Piece/Queen.h"
#include "Piece/King.h"

template<typename T>
static void addPiece(Board& board, const Point& position, Piece::Team team)
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

		board[whitePos] = std::make_unique<Pawn>(whitePos, Piece::White, currentTurn);
		board[blackPos] = std::make_unique<Pawn>(blackPos, Piece::Black, currentTurn);
	}

	// Setup pieces
	for (int i = 0; i < Piece::MaxTeams; ++i)
	{
		// 0 for white, 7 for black
		int rank = i * (Settings::g_boardSize - 1);
		Piece::Team team{ static_cast<Piece::Team>(i) };
		Piece::Team opponent{ static_cast<Piece::Team>((i + 1) % Piece::MaxTeams) };

		addPiece<Rook>(board, { rank, 0 }, team);
		addPiece<Knight>(board, { rank, 1 }, team);
		addPiece<Bishop>(board, { rank, 2 }, team);
		addPiece<Queen>(board, { rank, 3 }, team);

		// Create a new king
		auto king = std::make_unique<King>(Point{ rank, 4 }, team, attackBoard);
		// Save a raw pointer to view the king later
		kings[i] = king.get();
		// Move it onto the board
		board[{rank, 4}] = std::move(king);


		addPiece<Bishop>(board, { rank, 5 }, team);
		addPiece<Knight>(board, { rank, 6 }, team);
		addPiece<Rook>(board, { rank, 7 }, team);
	}
}

bool Game::isInCheck(Piece::Team team)
{
	Piece::Team oppTeam{ static_cast<Piece::Team>((currentTeam + 1) % Piece::MaxTeams) };
	return attackBoard.isAttacking(kings[currentTeam]->getPosition(), oppTeam);
}

void Game::playGame()
{
	Board& board = getBoard();
	InputResult start{};
	while (start.result != InputResult::QUIT)
	{
		static std::unique_ptr<Move> savedMove{};

		currentTeam = static_cast<Piece::Team>((currentTurn) % Piece::MaxTeams);
		Piece::Team oppTeam{ static_cast<Piece::Team>((currentTeam + 1) % Piece::MaxTeams) };
		
		attackBoard.update(board, currentTeam, kings);
		//attackBoard.printBoard(std::cout, oppTeam);

		std::cout << '\n' << board << '\n';

		// Check for end conditions
		if (kings[currentTeam]->getPossibleMoves(board).size() == 0 && !hasPossibleNonKingMove(currentTeam))
		{
			if (isInCheck(currentTeam))
			{
				// The winner will be the player of last turn
				std::cout << "CHECKMATE! " << (currentTeam ? "White" : "Black") << " wins!\n\n";
				break;
			}
			else
			{
				std::cout << "Stalemate.\n\n";
				break;
			}
		}
		else if (isInCheck(currentTeam))
		{
			std::cout << "CHECK!\n\n";
		}
	
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
				std::unique_ptr<Move> move = std::move(moveHistory.back());
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

		if (!processTurn(start.point, end.point, true, &Input::getPromotionType))
		{
			std::cout << "Invalid move.\n";
		}
	}
}

// Processes a turn given valid start and end inputs
bool Game::processTurn(const Point& start, const Point& end, bool printMove, std::function<char()> getExtraInput)
{
	// Check if the input is a valid move of the selected piece
	MoveSet possibleMoves{ board[start]->getPossibleMoves(board) };
	auto moveItr{ possibleMoves.find(end) };

	if (moveItr == possibleMoves.end())
	{
		return false;
	}

	moveItr->second->executeMove(board);

	// Check if the moved piece was pinned
	attackBoard.update(board, currentTeam, kings);
	moveItr->second->undoMove(board);
	if (isInCheck(currentTeam))
	{
		return false;
	}

	if (printMove)
	{
		std::cout << '\n';
		moveItr->second->printMove();
	}

	// Complete move
	moveItr->second->executeMove(board, getExtraInput);

	// Go to next turn
	moveHistory.push_back(std::move(moveItr->second));
	++currentTurn;
	return true;
}

// Returns true if the specified player has a valid piece move on the current board
bool Game::hasPossibleNonKingMove(Piece::Team team)
{
	Piece::Team opp{ static_cast<Piece::Team>((currentTeam + 1) % Piece::MaxTeams) };
	AttackBoard attackBoard{};

	for (int rank = 0; rank < Settings::g_boardSize; ++rank)
	{
		for (int file = 0; file < Settings::g_boardSize; ++file)
		{
			Point pos{ rank, file };
			if (!isAlliedPiece(board, pos, team) || board[pos]->getType() == Piece::King) { continue; }
			
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
