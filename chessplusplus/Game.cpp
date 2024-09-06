#include "Game.h"
#include "Settings.h"
#include "Pawn.h"

Game::Game()
{
	// Setup white pawns
	for (int i = 0; i < Settings::g_boardSize; ++i)
	{
		Point pos{ Settings::g_boardSize - 2, i };
		board[pos] = std::make_unique<Piece>(pos, Piece::White);
	}
}