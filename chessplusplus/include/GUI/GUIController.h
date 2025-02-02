#pragma once

#include <memory>

#include "GUI/Object.h"
#include "GUI/Chessboard.h"
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

	private:
		std::shared_ptr<Object> mainMenu;
		std::shared_ptr<Object> gameMenu;
		std::shared_ptr<Chessboard> chessboard;

		std::unique_ptr<Game> game{};

	public:
		void setMainMenu(std::shared_ptr<Object> menu) { mainMenu = menu; }
		void setGameMenu(std::shared_ptr<Object> menu) { gameMenu = menu; }
		void setBoard(std::shared_ptr<Chessboard> board) { chessboard = board; }
	};
}