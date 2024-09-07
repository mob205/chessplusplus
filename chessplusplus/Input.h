#pragma once
#include <iostream>
#include <sstream>

#include "Point.h"
#include "InputResult.h"

namespace Input
{
	inline void displayMainMenu()
	{
		std::cout << "A. Start new game\n"
			<< "B. Load a saved game\n"
			<< "C. Quit\n";
	}
	inline bool isValidMenuInput(char input)
	{
		return input == 'A' || input == 'B' || input == 'C';
	}
	inline char getMenuInput()
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

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return input;
		}
	}

	inline InputResult getTileInput()
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
}