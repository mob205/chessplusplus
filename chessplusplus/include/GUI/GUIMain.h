#pragma once
#include "SFML/Graphics.hpp"
#include "Game/Settings.h"


namespace GUI
{
	inline constexpr int buttonWidth{ 250 };
	inline constexpr int buttonHeight{ 75 };
	inline constexpr int pixelsPerTile{ 65 };
	inline constexpr int boardLength{ Settings::boardSize * pixelsPerTile };
	inline constexpr int menuSize{ 1200 };
	inline constexpr float startSizeX{ 1080.f };
	inline constexpr float startSizeY{ 900.f };

	const sf::Color buttonColor{ 215, 215, 215 };

	const sf::Color teamWhiteColor{ 220, 220, 220 };
	const sf::Color teamBlackColor{ 100,100,100 };

	void startGUI();
}
