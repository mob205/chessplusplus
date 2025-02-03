#include "SFML/Graphics.hpp"
#include <memory>
#include "GUI/GUIController.h"

#include <iostream>

namespace GUI
{
	void GUIController::onStart()
	{
		mainMenu->setActive(false);
		gameMenu->setActive(true);

		chessboard->updateBoard(game->getBoard());
	}

	void GUIController::onQuit()
	{
		gameMenu->setActive(false);
		mainMenu->setActive(true);

		// Reset game
		game = std::make_unique<Game>();
	}

	void GUIController::onUndo()
	{
		game->undoMove();
		chessboard->updateBoard(game->getBoard());
	}

	void GUIController::onLoad()
	{

	}

	void GUIController::onSave()
	{

	}

	void GUIController::unselect()
	{
		chessboard->unhighlightAllTiles();
		hasSelected = false;
	}

	void GUIController::selectPieceTile(Point pos)
	{
		chessboard->unhighlightAllTiles();
		chessboard->highlightTile(pos, true);

		Piece* piece = game->getBoard()[pos].get();

		for (const auto& move : piece->getPossibleMoves(game->getBoard()))
		{
			if (game->isValidMove(move.second->getStart(), move.first))
			{
				chessboard->highlightTile(move.first, true);
			}
		}

		currentSelection = pos;
		hasSelected = true;
	}

	void GUIController::onTileSelected(Point pos)
	{
		const Board& board = game->getBoard();
		Team curTeam = game->getCurrentTeam();

		if (!hasSelected)
		{
			// Nothing selected
			if (!board[pos]) { return; }

			// Piece is not owned by current player
			if (board[pos]->getTeam() != curTeam) { return; }

			// Selected piece
			selectPieceTile(pos);
			return;
		}

		// A piece is currently selected

		// Selected an empty or enemy tile - attempt to move there
		if (!board[pos] || board[pos]->getTeam() != curTeam)
		{
			game->processTurn(currentSelection, pos);
			chessboard->updateBoard(board);

			unselect();
			return;
		}

		// Selected different owned piece - change selection
		if (board[pos]->getTeam() == curTeam)
		{
			selectPieceTile(pos);
			return;
		}

		// Selected the same piece - unselect
		if (pos == currentSelection)
		{
			unselect();
			return;
		}
	}
	void GUIController::setBoard(std::shared_ptr<Chessboard> board)
	{
		chessboard = board;
		chessboard->setOnTileInteracted([=](Point pos) { onTileSelected(pos); });
	}
}

