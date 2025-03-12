#include <memory>
#include <format>
#include <iostream>

#include "SFML/Graphics.hpp"

#include "GUI/GUIController.h"
#include "Board/BoardHelpers.h"
#include "Player/ChessPlayer.h"
#include "Player/EnginePlayer.h"

namespace GUI
{
	void GUIController::startLocalGame()
	{
		players[PieceEnums::White] = std::make_unique<ChessPlayer>(PieceEnums::White);
		players[PieceEnums::Black] = std::make_unique<ChessPlayer>(PieceEnums::Black);

		startGame();
	}

	void GUIController::startEngineGame(PieceEnums::Team localPlayerTeam)
	{
		players[localPlayerTeam] = std::make_unique<ChessPlayer>(localPlayerTeam);

		PieceEnums::Team opp = getOppositeTeam(localPlayerTeam);
		players[opp] = std::move(std::make_unique<EnginePlayer>(opp));

		startGame();
	}

	void GUIController::startGame()
	{
		mainMenu->setActive(false);
		gameMenu->setActive(true);

		// Cache this, as engine thinking will modify game state
		activePlayerTeam = game->getCurrentTeam();

		if (players[activePlayerTeam]->usesGUI())
		{
			chessboard->setOrientation(activePlayerTeam);
		}
		else
		{
			chessboard->setOrientation(getOppositeTeam(activePlayerTeam));
		}

		log->clearMessages();
		log->logMessage("Welcome to Chess!");

		resetTempState();

		players[activePlayerTeam]->startTurn(game.get());
	}

	void GUIController::onQuit()
	{
		if (!players[activePlayerTeam]->usesGUI()) { return; }

		gameMenu->setActive(false);
		mainMenu->setActive(true);

		// Reset game
		game = std::make_unique<Game>();
	}

	void GUIController::onUndo()
	{
		// Don't undo if waiting for active player
		if (!players[activePlayerTeam]->usesGUI()) { return; }

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
		// Don't undo if waiting for active player
		if (!players[activePlayerTeam]->usesGUI()) { return; }

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
		if (!players[activePlayerTeam]->usesGUI()) { return; }

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
		if (isGameOver || isPromoting || !players[activePlayerTeam]->usesGUI()) { return; }

		const Board& board = game->getBoard();

		if (!hasSelected)
		{
			// Nothing selected
			if (!board[pos]) { return; }

			// Piece is not owned by current player
			if (board[pos]->getTeam() != activePlayerTeam) { return; }

			// Selected piece
			selectPieceTile(pos);
			return;
		}

		// A piece is currently selected

		// Selected an empty or enemy tile - attempt to move there
		if (!board[pos] || board[pos]->getTeam() != activePlayerTeam)
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
		if (board[pos]->getTeam() == activePlayerTeam)
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


	void GUIController::handleMoveSuccess(const MoveResult& move)
	{
		if (players[activePlayerTeam]->usesGUI())
		{
			players[activePlayerTeam]->onLocalMoveSelected({ move.start, move.end, move.promotion.promotionType });
		}
		log->logMove(move);

		switch (move.oppStatus)
		{
		case MoveResult::OpponentStatus::Check:
			log->logMessage("Check!");
			break;

		case MoveResult::OpponentStatus::Checkmate:
		{
			std::string_view winningTeam = (activePlayerTeam == PieceEnums::White ? "White" : "Black");
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
			updateTurnCount();
			players[activePlayerTeam]->startTurn(game.get());
		}
		chessboard->updateBoard(game->getBoard());
	}

	void GUIController::resetTempState()
	{
		isGameOver = false;
		isPromoting = false;
		promoMenu->setActive(false);

		unselect();
		updateTurnCount();
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

	void GUIController::updateTurnCount()
	{
		activePlayerTeam = game->getCurrentTeam();
		std::string_view teamString{ activePlayerTeam == PieceEnums::White ? "White" : "Black" };

		log->logMessage("");
		log->logMessage(std::format("{}'s Turn", teamString));

		if (players[activePlayerTeam]->usesGUI())
		{
			chessboard->setOrientation(activePlayerTeam);
		}

		// A turn is only completed when both players have moved
		int currentTurn = (game->getCurrentTurn() / 2) + 1;

		turnCounter->setText(std::format("Turn {} | {}'s Turn", currentTurn, teamString));

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

	void GUIController::tick()
	{
		if (!players[activePlayerTeam]) { return; }
		MoveInput move;
		
		if (players[activePlayerTeam]->getMove(move))
		{
			if (MoveResult res = game->processTurn(move.start, move.end, move.extraInput))
			{
				handleMoveSuccess(res);
			}
			else
			{
				std::cerr << "Player asynchronously returned invalid move!";
			}
		}
	}
}

