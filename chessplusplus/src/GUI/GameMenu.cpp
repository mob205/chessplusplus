#include "GUI/GameMenu.h"
#include "GUI/GUIMain.h"

#include "SFML/Graphics.hpp"

#include "Game/Game.h"
#include "Board/Board.h"
#include "Game/Settings.h"
#include "Game/Point.h"

#include "Game/GameSerializer.h"

static Point numToPoint(int i)
{
	return { i / Settings::boardSize, i % Settings::boardSize };
}


GameMenu::GameMenu(sf::Vector2f size, std::vector<std::vector<sf::Texture>>& pieceTextures)
	: Menu{ size }, pieceTextures{pieceTextures}
{
	promotionTexture.create(500, 300);
}

void GameMenu::addTileButton(std::unique_ptr<TileButton> tile)
{
	tiles.emplace_back(std::move(tile));
}

void GameMenu::addPromoButton(std::unique_ptr<PromoButton> promoButton)
{
	promoButtons.emplace_back(std::move(promoButton));
}

bool GameMenu::onButtonPress(sf::Vector2f point)
{
	if (Menu::onButtonPress(point))
	{
		return true;
	}

	for (int i = 0; i < promoButtons.size(); ++i)
	{
		if (promoButtons[i]->containsPoint(point) && isPromoting)
		{
			promoButtons[i]->onClick();
			return true;
		}
	}
	bool clickedTile{};
	for (int i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i]->containsPoint(point))
		{
			selectPiece(i);
			clickedTile = true;
			break;
		}
	}
	if (!clickedTile && !isPromoting && !isGameDone)
	{
		unselectPiece(true);
		return false;
	}
	return true;
}

void GameMenu::selectPiece(int i)
{
	if (isPromoting || isGameDone) { return; }
	Point sel{ numToPoint(i) };
	const Board& board{ game->getBoard() };
	if (selectedPiece == -1 && (!board[sel] || board[sel]->getTeam() != game->getCurrentTeam()))
	{
		return;
	}
	if (selectedPiece == -1 || (selectedPiece != -1 && board[sel] && board[sel]->getTeam() == game->getCurrentTeam()))
	{
		selectedPiece = i;
		eventLog->setString("Selected a " + std::string(board[sel]->getName()) + "\nSelect a tile to move to.\n");
		return;
	}
	Point start{ numToPoint(selectedPiece) };
	MoveResult res{ game->processTurn(start, sel, '\0') };

	if (res.reasonFailed == MoveResult::MoveFailReason::NeedsInput)
	{
		isPromoting = true;
		promoStart = start;
		promoEnd = sel;
		eventLog->setString("Choose a piece to promote to.\n");
		return;
	}
	logMove(res);

	updateBoard();
	updateTurnCounter();
	unselectPiece(false);
}

void GameMenu::unselectPiece(bool clearLog)
{
	selectedPiece = -1;
	eventLog->setString((clearLog ? "" : eventLog->getString()) + "Select a piece to move.\n");
}

void GameMenu::logMove(MoveResult res)
{
	eventLog->setString("");
	if (!res)
	{
		switch (res.reasonFailed)
		{
		case MoveResult::MoveFailReason::InvalidPiece:
			eventLog->setString("Invalid move. There is no owned piece on this tile.\n");
			break;
		case MoveResult::MoveFailReason::Check:
			eventLog->setString("Invalid move. King in check must be defended.\n");
			break;
		case MoveResult::MoveFailReason::Pinned:
			eventLog->setString("Invalid move. This piece is pinned and must defend the king.\n");
			break;
		case MoveResult::MoveFailReason::NotInSet:
			eventLog->setString("Invalid move.\n");
			break;
		}
		return;
	}

	switch (res.type)
	{
	case MoveResult::Type::Standard:
		if (res.standard.capturedPiece)
		{
			eventLog->setString("Captured an enemy " + std::string(PieceEnums::pieceNames[res.standard.capturedPiece]) + "\n");
		}
		break;
	case MoveResult::Type::Castle:
		eventLog->setString("Castled!\n");
		break;
	case MoveResult::Type::EnPassant:
		eventLog->setString(eventLog->getString() + "EN PASSANT!!\n");
		break;
	case MoveResult::Type::Promotion:
		if (res.promotion.capturedPiece)
		{
			eventLog->setString("Captured an enemy " + std::string(PieceEnums::pieceNames[res.promotion.capturedPiece]) + "\n");
		}
		eventLog->setString(eventLog->getString() + "Promoted pawn to " + std::string(PieceEnums::pieceNames[res.promotion.promotionType]) + "\n");
		break;
	}

	switch (res.oppStatus)
	{
	case MoveResult::OpponentStatus::Check:
		eventLog->setString(eventLog->getString() + "Check!\n");
		break;
	case MoveResult::OpponentStatus::Checkmate:
		eventLog->setString(eventLog->getString() + "CHECKMATE! " + (game->getCurrentTeam() ? "Black" : "White") + " WINS!!\n");
		isGameDone = true;
		return;
	case MoveResult::OpponentStatus::Stalemate:
		eventLog->setString(eventLog->getString() + "Stalemate.\n");
		isGameDone = true;
		return;
	}
}

void GameMenu::insertPromoInput(char input)
{
	logMove(game->processTurn(promoStart, promoEnd, input));
	isPromoting = false;

	updateBoard();
	updateTurnCounter();
	unselectPiece(false);
}

void GameMenu::onActive()
{
	game = std::make_unique<Game>();
	isGameDone = false;
	updateBoard();
	updateTurnCounter();
	unselectPiece(true);
	updatePromo();
}

void GameMenu::undo()
{
	if (isPromoting) { return; }
	if (game->undoMove())
	{
		eventLog->setString("Undo successful.\n");
		isGameDone = false;
		updateBoard();
		updateTurnCounter();
	}
	else
	{
		eventLog->setString("No move left to undo.\n");
	}
}

void GameMenu::updateBoard()
{
	const Board& board{ game->getBoard() };
	for (int i = 0; i < Settings::boardSize; ++i)
	{
		for (int j = 0; j < Settings::boardSize; ++j)
		{
			int tileIdx = i * Settings::boardSize + j;
			Piece* piece{ board[{i, j}].get() };
			if (piece != nullptr)
			{ 
				tiles[tileIdx]->setPieceSprite(&pieceTextures[piece->getTeam()][piece->getType()]);
			}
			else
			{
				tiles[tileIdx]->resetPieceSprite();
			}
		}
	}

}

void GameMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Menu::draw(target, states);
	for (int i = 0; i < tiles.size(); ++i)
	{
		target.draw(*tiles[i]);
	}
	if (eventLog)
	{
		target.draw(*eventLog);
	}
	if (turnCounter)
	{
		target.draw(*turnCounter);
	}
	if (saveFileText)
	{
		target.draw(*saveFileText);
	}
	if (isPromoting)
	{
		target.draw(promoSprite);
	}
}

void GameMenu::onResize(sf::Vector2f center)
{
	for (int i = 0; i < tiles.size(); ++i)
	{
		tiles[i]->recenter(center);
	}
	if (eventLog)
	{
		eventLog->setPosition(logOffset + center);
	}
	if (turnCounter)
	{
		turnCounter->setPosition(counterOffset + center);
	}
	if (saveFileText)
	{
		saveFileText->setPosition(saveFileOffset + center);
	}
	promoSprite.setPosition(promotionOffset + center);
	for (int i = 0; i < promoButtons.size(); ++i)
	{
		promoButtons[i]->recenter(promoSprite.getPosition());
	}
}

void GameMenu::setLogText(std::unique_ptr<sf::Text> log)
{
	eventLog = std::move(log);
	logOffset = eventLog->getPosition() - sf::Vector2f{ GUI::menuSize / 2, GUI::menuSize / 2 } + sf::Vector2f{ 0, 100 }; 
	// sf::Vector2f{0, 100} is the difference between window and menu size, divided by 2. TODO: make this not a magic number
}

void GameMenu::setTurnCounter(std::unique_ptr<sf::Text> counter)
{
	turnCounter = std::move(counter);
	counterOffset = turnCounter->getPosition() - sf::Vector2f{ GUI::menuSize / 2, GUI::menuSize / 2 } + sf::Vector2f{ 0, 100 };
}

void GameMenu::setSaveText(std::unique_ptr<sf::Text> saveText)
{
	saveFileText = std::move(saveText);
	saveFileOffset = saveFileText->getPosition() - sf::Vector2f{ GUI::menuSize / 2, GUI::menuSize / 2 } + sf::Vector2f{ 0, 100 };
}

void GameMenu::updateTurnCounter()
{
	turnCounter->setString("Turn " + std::to_string((game->getCurrentTurn() / 2) + 1) + " | " + (game->getCurrentTeam() ? "Black" : "White") + " to play");
}

sf::RenderTexture& GameMenu::getPromoTexture()
{
	return promotionTexture;
}

void GameMenu::updatePromo()
{
	promotionTexture.display();
	promoSprite.setTexture(promotionTexture.getTexture());
	promoSprite.setPosition({promoInitPos});
	promotionOffset = promoInitPos - sf::Vector2f{ GUI::menuSize / 2, GUI::menuSize / 2 } + sf::Vector2f{ 0, 100 };
	for (int i = 0; i < promoButtons.size(); ++i)
	{
		promoButtons[i]->recenter(promoInitPos);
	}
}

void GameMenu::onType(sf::Uint32 unicode)
{
	if (unicode == '\b' && currentSaveFile.size() > 0)
	{
		currentSaveFile.erase(currentSaveFile.end() - 1);
	}
	else if (unicode != '\b' && currentSaveFile.size() < 25)
	{
		currentSaveFile += unicode;
	}
	saveFileText->setString(currentSaveFile);
}

void GameMenu::loadGame()
{
	if (currentSaveFile.size() == 0 || isPromoting) { return; }
	auto newGame{ std::make_unique<Game>() };
	GameSerializer::LoadGameResult res{ newGame->getSerializer().loadGame(currentSaveFile)};
	switch (res)
	{
	case GameSerializer::LoadSuccessful:
		eventLog->setString("Successfully loaded save.\n");
		game = std::move(newGame);
		isGameDone = false;
		updateBoard();
		updateTurnCounter();
		break;
	case GameSerializer::SaveNotFound:
		eventLog->setString("Save not found.\n");
		break;
	case GameSerializer::SaveInvalid:
		eventLog->setString("Save invalid.\n");
		break;
	}
}

void GameMenu::saveGame()
{
	if (currentSaveFile.size() == 0 || isGameDone || isPromoting) { return; }
	if (game->getSerializer().saveGame(currentSaveFile))
	{
		eventLog->setString("Successfully saved game.\n");
	}
	else
	{
		eventLog->setString("Failed to save game.\n");
	}
}