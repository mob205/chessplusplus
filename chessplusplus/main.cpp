#include <iostream>
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
			// Start a game
			break;
		case 'B':
			// Load a game and play it
			break;
		case 'C':
			std::cout << "Goodbye!";
			break;
		}
	}
}