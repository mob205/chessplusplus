#include "SFML/Graphics.hpp"
#include <memory>
#include "GUI/GUIController.h"


namespace GUI
{
	void GUIController::onStart()
	{
		mainMenu->setVisibility(false);
		gameMenu->setVisibility(true);
	}

	void GUIController::onQuit()
	{
		gameMenu->setVisibility(false);
		mainMenu->setVisibility(true);

		// Reset game
		game = std::make_unique<Game>();
	}

	void GUIController::onUndo()
	{

	}

	void GUIController::onLoad()
	{

	}

	void GUIController::onSave()
	{

	}
}

