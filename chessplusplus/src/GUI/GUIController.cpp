#include "SFML/Graphics.hpp"
#include <memory>
#include "GUI/GUIController.h"


namespace GUI
{
	void GUIController::onStart()
	{
		mainMenu->setActive(false);
		gameMenu->setActive(true);
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

	}

	void GUIController::onLoad()
	{

	}

	void GUIController::onSave()
	{

	}
}

