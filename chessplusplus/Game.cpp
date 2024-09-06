#include "Game.h"
#include "Settings.h"
#include "Pawn.h"
#include "Input.h"
#include "BoardHelpers.h"

Game::Game()
{
	// Setup pawns pawns
	for (int i = 0; i < Settings::g_boardSize; ++i)
	{
		Point whitePos{ Settings::g_boardSize - 2, i };
		Point blackPos{ 1, i };

		board[whitePos] = std::make_unique<Pawn>(whitePos, Piece::White);
		board[blackPos] = std::make_unique<Pawn>(blackPos, Piece::Black);
	}
}

void playGame(Game& game)
{
	Board& board = game.getBoard();
	InputResult res{};
	while (res.result != InputResult::QUIT)
	{
		std::cout << board << '\n';
		std::cout << "Select a piece to move, or type 'QUIT' to quit.\n";

		InputResult res = Input::getGameInput();
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
		std::cout << board[res.point]->getSymbol();

	}
}