#include "SFML/Graphics.hpp"
#include <memory>
#include "GUI/GUIController.h"
#include <format>
#include <iostream>

namespace GUI
{
	void GUIController::startGame(PieceEnums::Team team)
	{
		mainMenu->setActive(false);
		gameMenu->setActive(true);

		localPlayerTeam = team;
		chessboard->setOrientation(localPlayerTeam);

		log->clearMessages();
		log->logMessage("Welcome to Chess!");

		if (game->getCurrentTeam() != localPlayerTeam)
		{
			handleAIMove();
		}

		resetTempState();
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
		if (game->undoMove())
		{
			log->logMessage("Move undone.");

			resetTempState();
		}
		else
		{
			log->logMessage("Nothing left to undo.");
		}
	}

	void GUIController::onLoad()
	{
		if (saveTextBox->getText().getSize() == 0) { return; }
		auto newGame = std::make_unique<Game>();
		GameSerializer::LoadGameResult loadResult = newGame->getSerializer().loadGame(saveTextBox->getText());
		if (loadResult == GameSerializer::LoadSuccessful)
		{
			game = std::move(newGame);
			log->logMessage("Game succesfully loaded.");
			resetTempState();
		}
		else if (loadResult == GameSerializer::SaveInvalid)
		{
			log->logMessage("Save is invalid.");
			log->logMessage("Game could not be loaded.");
		}
		else if (loadResult == GameSerializer::SaveNotFound)
		{
			log->logMessage("Save file not found.");
		}
	}

	void GUIController::onSave()
	{
		if (saveTextBox->getText().getSize() == 0) { return; }
		game->getSerializer().saveGame(saveTextBox->getText());
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
		if (isGameOver || isPromoting || game->getCurrentTeam() != localPlayerTeam) { return; }

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
			MoveResult res = game->processTurn(currentSelection, pos);
			if(res)
			{
				handleMoveSuccess(res);
			}
			else if (res.reasonFailed == MoveResult::MoveFailReason::NeedsInput)
			{
				promoMenu->setActive(true);
				isPromoting = true;
				promoStart = currentSelection;
				promoEnd = pos;
			}
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
	
	void GUIController::handleAIMove()
	{
		if (game->getCurrentTeam() != localPlayerTeam)
		{
			auto move = Engine::generateMove(game.get(), game->getCurrentTeam());

			if (MoveResult res = game->processTurn(move.first, move.second, 'Q'))
			{
				handleMoveSuccess(res);
			}
			else
			{
				std::cerr << "AI attempted an invalid move!\n";
			}
		}
	}

	void GUIController::handleMoveSuccess(const MoveResult& move)
	{
		log->logMove(move);
		chessboard->updateBoard(game->getBoard());

		switch (move.oppStatus)
		{
		case MoveResult::OpponentStatus::Check:
			log->logMessage("Check!");
			break;

		case MoveResult::OpponentStatus::Checkmate:
		{
			std::string_view winningTeam = (game->getCurrentTeam() ? "White" : "Black");
			log->logMessage(std::format("CHECKMATE! {} WINS!!", winningTeam));
			isGameOver = true;
		}
		break;

		case MoveResult::OpponentStatus::Stalemate:
			log->logMessage("Stalemate.");
			isGameOver = true;
			break;
		}

		if (!isGameOver)
		{
			updateTurnCounter();

			handleAIMove();
		}
	}

	void GUIController::resetTempState()
	{
		isGameOver = false;
		isPromoting = false;
		promoMenu->setActive(false);

		unselect();
		updateTurnCounter();
		chessboard->updateBoard(game->getBoard());
	}

	void GUIController::setBoard(std::shared_ptr<Chessboard> board)
	{
		chessboard = board;
		chessboard->setOnTileInteracted([=](Point pos) { onTileSelected(pos); });
	}

	void GUIController::setSaveBox(std::shared_ptr<TextObject> saveBox)
	{
		saveTextBox = saveBox;
	}

	void GUIController::updateTurnCounter()
	{
		std::string_view team{ game->getCurrentTeam() == PieceEnums::White ? "White" : "Black" };

		log->logMessage("");
		log->logMessage(std::format("{}'s Turn", team));

		// A turn is only completed when both players have moved
		int currentTurn = (game->getCurrentTurn() / 2) + 1;

		turnCounter->setText(std::format("Turn {} | {}'s Turn", currentTurn, team));

		// Center turn counter based on text size
		turnCounter->setPosition({ -turnCounter->getTextSize().x / 2.f, 0 });
	}

	void GUIController::onSelectPromotion(char promoInput)
	{
		promoMenu->setActive(false);
		isPromoting = false;

		MoveResult res = game->processTurn(promoStart, promoEnd, promoInput);
		if (!res)
		{
			std::cerr << "Promotion failed. This shouldn't ever happen.\n";
			return;
		}
		handleMoveSuccess(res);
	}

	void GUIController::onTextEntered(sf::Uint32 input)
	{
		std::string saveText = saveTextBox->getText();
		if (input == '\b' && saveText.size() > 0)
		{
			saveText.erase(saveText.end() - 1);
			saveTextBox->setText(saveText);
		}
		else if (input != '\b' && saveText.size() < 20)
		{
			saveText += input;
			saveTextBox->setText(saveText);
		}
	}
}

