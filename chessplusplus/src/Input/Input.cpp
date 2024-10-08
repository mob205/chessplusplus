#include "ConsoleInput/Input.h"
#include "Game/Point.h"

namespace IO
{
	void displayMainMenu()
	{
		std::cout << "A. Start new game\n"
			<< "B. Load a saved game\n"
			<< "C. Quit\n";
	}

	bool isValidMenuInput(char input)
	{
		return input == 'A' || input == 'B' || input == 'C';
	}

	char getMenuInput()
	{
		displayMainMenu();

		char input{};
		while (true)
		{
			std::cin >> input;

			if (!std::cin || !isValidMenuInput(input))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				std::cout << "Invalid input.\n";
				continue;
			}

			return input;
		}
	}

	InputResult getTileInput()
	{
		while (true)
		{
			std::string rawInput{};
			std::cin >> rawInput;

			if (rawInput == "QUIT")
			{
				return { {}, InputResult::QUIT };
			}
			else if (rawInput == "SAVE")
			{
				return { {}, InputResult::SAVE };
			}
			else if (rawInput == "UNDO")
			{
				return { {}, InputResult::UNDO };
			}
			std::stringstream inStream{ rawInput };

			Point point{};
			if (inStream >> point && point.isInBounds())
			{
				return { point, InputResult::POINT };
			}

			std::cout << "Invalid input.\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	char getPromotionType()
	{
		std::cout << "\nSelect a piece to promote to.\nQ - Queen | R - Rook | B - Bishop | N - Knight\n";
		while (true)
		{
			char input{};
			std::cin >> input;
			
			if (input == 'Q' || input == 'N' || input == 'B' || input == 'R')
			{
				return input;
			}

			std::cout << "Invalid input.\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	std::string getSaveName()
	{
		std::string input{};
		while (true)
		{
			std::cout << "Enter name of the save file: ";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::getline(std::cin, input);
			if (input != "") { return input; }
			std::cout << "Invalid input.\n";
		}
	}
}