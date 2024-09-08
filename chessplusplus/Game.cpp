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

void playGame(Game& game)
{
	Board& board = game.getBoard();
	InputResult res{};
	while (res.result != InputResult::QUIT)
	{
		game.currentTeam = static_cast<Piece::Team>((game.currentTurn) % Piece::MaxTeams);
		Piece::Team opp = static_cast<Piece::Team>((game.currentTeam + 1) % Piece::MaxTeams);
		
		game.attackBoard.update(board, game.currentTeam, game.currentTurn, game.kings);

		//std::cout << game.attackBoard;

		std::cout << '\n' << board << '\n';
		std::cout << (game.currentTeam ? "Black" : "White") << "'s turn!\n";
		std::cout << "Select a piece to move, or type 'QUIT' to quit.\n";


		res = Input::getTileInput();
		if (res.result == InputResult::QUIT)
		{
			break;
		}

		if (res.result == InputResult::SAVE)
		{
			// Saving stuff
			break;
		}

		// Must be point result
		if (!ownsPiece(board, res.point, game.currentTeam))
		{
			std::cout << "No owned piece is on this square!\n";
			continue;
		}
		
		Point start{ res.point };

		std::cout << "Selected a " << board[start]->getName() << '\n';


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

		moveItr->second->ExecuteMove(board);
		++game.currentTurn;
	}
}