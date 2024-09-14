#pragma once
#include <iostream>
#include <sstream>

#include "ConsoleInput/InputResult.h"

namespace IO
{
	void displayMainMenu();
	
	bool isValidMenuInput(char input);

	char getMenuInput();

	InputResult getTileInput();

	char getPromotionType();

	std::string getSaveName();
}