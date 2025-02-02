#include "SFML/Graphics.hpp"
#include <memory>
#include "GUI/GUIController.h"


namespace GUI
{
	void GUIController::onStart()
	{
		mainMenu->setVisibility(false);
		mainMenu->setInteractable(false);

		gameMenu->setVisibility(true);
		gameMenu->setInteractable(true);
	}

	void GUIController::onQuit()
	{
		gameMenu->setVisibility(false);
		gameMenu->setInteractable(false);

		mainMenu->setVisibility(true);
		mainMenu->setInteractable(true);

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

