#include <iostream>

#include "Game/Game.h"
#include "ConsoleInput/Input.h"
#include "ConsoleInput/ConsolePlayer.h"
#include "Board/Board.h"


void StartGameFromSave()
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
			StartGameFromSave();
			break;
		case 'C':
			std::cout << "Goodbye!";
			break;
		}
	}
}

int main()
{
	StartConsoleGame();
}



