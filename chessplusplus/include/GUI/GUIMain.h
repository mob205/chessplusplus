#pragma once
#include "Game/Settings.h"

namespace GUI
{
	inline constexpr int buttonWidth{ 250 };
	inline constexpr int buttonHeight{ 75 };
	inline constexpr int pixelsPerTile{ 75 };
	inline constexpr int boardLength{ Settings::boardSize * pixelsPerTile };
	inline constexpr int menuSize{ 1200 };

	void startGUI();
}
