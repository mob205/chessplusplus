#include "Game.h"
#include "Settings.h"
#include "Input.h"
#include "BoardHelpers.h"

#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h";
#include "Queen.h"

template<typename T>
static void addPiece(Board& board, const Point& position, int team)
{
	board[position] = std::make_unique<T>(position, static_cast<Piece::Team>(team));
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

		addPiece<Rook>(board, { rank, 0 }, i);
		addPiece<Knight>(board, { rank, 1 }, i);
		addPiece<Bishop>(board, { rank, 2 }, i);
		addPiece<Queen>(board, { rank, 3 }, i);

		addPiece<Bishop>(board, { rank, 5 }, i);
		addPiece<Knight>(board, { rank, 6 }, i);
		addPiece<Rook>(board, { rank, 7 }, i);
	}
}

void playGame(Game& game)
{
	Board& board = game.getBoard();
	InputResult res{};
	while (res.result != InputResult::QUIT)
	{
		game.currentTeam = static_cast<Piece::Team>((game.currentTurn) % Piece::MaxTeams);
		std::cout << '\n' << board << '\n';
		std::cout << (game.currentTeam ? "Black" : "White") << "'s turn!\n";
		std::cout << "Select a piece to move, or type 'QUIT' to quit.\n";

		res = Input::getTileInput();
		if (res.result == InputResult::QUIT)
		{
			// Quitting stuff
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