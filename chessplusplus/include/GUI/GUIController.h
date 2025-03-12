#pragma once

#include <memory>
#include <array>

#include "GUI/Object.h"
#include "GUI/Chessboard.h"
#include "GUI/TextObject.h"
#include "GUI/TurnLog.h"
#include "Game/Game.h"

#include "Engine/Engine.h"
#include "Player/ChessPlayer.h"

namespace GUI
{
	// Connects game logic with GUI
	class GUIController
	{
	public:
		GUIController()
			: game{ std::make_unique<Game>() }
		{}

		// Button callbacks
		void startLocalGame();
		void startEngineGame(PieceEnums::Team localPlayerTeam);
		void onQuit();
		void onUndo();
		void onLoad();
		void onSave();
		void onTileSelected(Point pos);
		void onSelectPromotion(char promoInput);
		void onTextEntered(sf::Uint32 input);

		void tick();

	private:
		std::shared_ptr<Object> mainMenu;
		std::shared_ptr<Object> gameMenu;
		std::shared_ptr<Chessboard> chessboard;
		std::shared_ptr<TextObject> turnCounter;
		std::shared_ptr<TurnLog> log;
		std::shared_ptr<Object> promoMenu;
		std::shared_ptr<TextObject> saveTextBox;

		std::unique_ptr<Game> game{};

		std::array<std::unique_ptr<ChessPlayer>, 2> players{};

		// The team who is to take the next turn
		PieceEnums::Team activePlayerTeam{};

		Point currentSelection{};
		bool hasSelected{};
		bool isGameOver{};

		bool isPromoting{};
		Point promoStart{};
		Point promoEnd{};

		void startGame();

		void unselect();
		void selectPieceTile(Point pos);
		void updateTurnCount();
		void handleMoveSuccess(const MoveResult& move);
		void resetTempState();

	public:
		void setMainMenu(std::shared_ptr<Object> menu) { mainMenu = menu; }
		void setGameMenu(std::shared_ptr<Object> menu) { gameMenu = menu; }
		void setTurnCounter(std::shared_ptr<TextObject> counter) { turnCounter = counter; }
		void setTurnLog(std::shared_ptr<TurnLog> turnLog) { log = turnLog; }
		void setBoard(std::shared_ptr<Chessboard> board);
		void setPromotionMenu(std::shared_ptr<Object> menu) { promoMenu = menu; }
		void setSaveBox(std::shared_ptr<TextObject> saveBox);
	};
}