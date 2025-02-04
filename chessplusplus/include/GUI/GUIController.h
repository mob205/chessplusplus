#pragma once

#include <memory>

#include "GUI/Object.h"
#include "GUI/Chessboard.h"
#include "GUI/TextObject.h"
#include "GUI/TurnLog.h"
#include "Game/Game.h"

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
		void onStart();
		void onQuit();
		void onUndo();
		void onLoad();
		void onSave();
		void onTileSelected(Point pos);

	private:
		std::shared_ptr<Object> mainMenu;
		std::shared_ptr<Object> gameMenu;
		std::shared_ptr<Chessboard> chessboard;
		std::shared_ptr<TextObject> turnCounter;
		std::shared_ptr<TurnLog> log;

		std::unique_ptr<Game> game{};

		Point currentSelection{};
		bool hasSelected{};
		bool isGameOver{};

		void unselect();
		void selectPieceTile(Point pos);
		void updateTurnCounter();
		void handleMoveSuccess(const MoveResult& move);

	public:
		void setMainMenu(std::shared_ptr<Object> menu) { mainMenu = menu; }
		void setGameMenu(std::shared_ptr<Object> menu) { gameMenu = menu; }
		void setTurnCounter(std::shared_ptr<TextObject> counter) { turnCounter = counter; }
		void setTurnLog(std::shared_ptr<TurnLog> turnLog) { log = turnLog; }
		void setBoard(std::shared_ptr<Chessboard> board);
	};
}