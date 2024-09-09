#pragma once
#include <iostream>
#include <sstream>

#include "Piece/Piece.h"
#include "Input/InputResult.h"

namespace Input
{
	void displayMainMenu();
	
	bool isValidMenuInput(char input);

	char getMenuInput();

	InputResult getTileInput();

	char getPromotionType();

	std::string getSaveName();
}