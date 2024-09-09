#include <iostream>

#include "Game.h"
#include "Input.h"
#include "Board.h"

int main()
{
	std::cout << "Welcome to Chess!\n\n";


	char input{};
	while (input != 'C')
	{
		input = Input::getMenuInput();
		switch (input)
		{
		case 'A':
		// Start a new game
		{
			Game game{};
			game.playGame();
		}
			break;
		case 'B':
		// Load a game and play it
		{
			Game game{};
			if (game.getSerializer().loadGame(Input::getSaveName()))
			{
				std::cout << "Successfully loaded game.\n";
				game.playGame();
			}
			else
			{
				std::cout << "Invalid save file.\n";
			}
		}
			break;
		case 'C':
			std::cout << "Goodbye!";
			break;
		}
	}
}

