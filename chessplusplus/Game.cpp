#include "Game.h"
#include "Settings.h"
#include "Input.h"
#include "BoardHelpers.h"

#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

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
	InputResult res{};
	while (res.result != InputResult::QUIT)
	{
		static std::unique_ptr<Move> savedMove{};

		currentTeam = static_cast<Piece::Team>((currentTurn) % Piece::MaxTeams);
		Piece::Team oppTeam{ static_cast<Piece::Team>((currentTeam + 1) % Piece::MaxTeams) };
		
		attackBoard.update(board, currentTeam, kings);

		std::cout << '\n' << board << '\n';

		// Check for end conditions
		if (kings[currentTeam]->getPossibleMoves(board).size() == 0 && !hasPossibleNonKingMove(currentTeam))
		{
			if (isInCheck(currentTeam))
			{
				// The winner will be the player of *last* turn
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


		res = Input::getTileInput();
		if (res.result == InputResult::QUIT)
		{
			break;
		}

		else if (res.result == InputResult::SAVE)
		{
			// Saving stuff
			break;
		}
		else if (res.result == InputResult::UNDO)
		{
			if (!moveHistory.empty())
			{
				std::unique_ptr<Move> move = std::move(moveHistory.top());
				moveHistory.pop();
				move->undoMove(board);
				std::cout << "Undo successful.\n";
			}
			else
			{
				std::cout << "No move to undo!\n";
			}
			continue;
		}

		// Must be point result
		if (!ownsPiece(board, res.point, currentTeam))
		{
			std::cout << "No owned piece is on this square!\n";
			continue;
		}
		
		Point start{ res.point };

		std::cout << "Selected a " << board[start]->getName() << "\n\n";


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


		MoveSet possibleMoves{ board[start]->getPossibleMoves(board) };
		auto moveItr{ possibleMoves.find(end.point) };


		if (moveItr == possibleMoves.end())
		{
			std::cout << "Invalid move.\n";
			continue;
		}

		moveItr->second->executeMove(board);
		moveItr->second->printMove();
		std::cout << '\n';

		attackBoard.update(board, currentTeam, kings);
		// Prevent moving pinned pieces
		if (isInCheck(currentTeam))
		{
			moveItr->second->undoMove(board);
			std::cout << "Invalid move.";
			continue;
		}

		moveHistory.push(std::move(moveItr->second));
		++currentTurn;
	}
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
				if (!attackBoard.isAttacking(kings[team]->getPosition(), opp))
				{
					return true;
				}
			}
		}
	}
	// Checked every move and none are valid
	return false;
}