#pragma once
#include "Game/Settings.h"

namespace GUI
{
	inline constexpr int buttonWidth{ 300 };
	inline constexpr int buttonHeight{ 100 };
	inline constexpr int pixelsPerTile{ 75 };
	inline constexpr int boardLength{ Settings::boardSize * pixelsPerTile };
	inline constexpr int menuSize{ 1000 };

	void startGUI();
}
