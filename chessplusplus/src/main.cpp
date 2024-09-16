#include <iostream>

#include "Game/Game.h"
#include "ConsoleInput/Input.h"
#include "ConsoleInput/ConsolePlayer.h"
#include "GUI/GUIMain.h"
#include "Board/Board.h"
#include "Game/Settings.h"

#include "SFML/Graphics.hpp"

void LoadConsoleGameFromSave();
void StartConsoleGame();

int main()
{
	//StartConsoleGame();
	GUI::startGUI();
    return 0;
}

void LoadConsoleGameFromSave()
{
	Game game{};
	GameSerializer::LoadGameResult res{ game.getSerializer().loadGame(IO::getSaveName()) };

	switch (res)
	{
	case GameSerializer::LoadSuccessful:
		std::cout << "Successfully loaded game.\n";
		IO::playGame(game);
		break;
	case GameSerializer::SaveNotFound:
		std::cout << "Save file does not exist.\n";
		break;
	case GameSerializer::SaveInvalid:
		std::cout << "Save file is invalid.\n";
		break;
	}
}

void StartConsoleGame()
{
	std::cout << "Welcome to Chess!\n\n";


	char input{};
	while (input != 'C')
	{
		input = IO::getMenuInput();
		switch (input)
		{
		case 'A':
			// Start a new game
		{
			Game game{};
			IO::playGame(game);
		}
		break;
		case 'B':
			LoadConsoleGameFromSave();
			break;
		case 'C':
			std::cout << "Goodbye!";
			break;
		}
	}
}