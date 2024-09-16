#pragma once
#include "SFML/Graphics.hpp"
#include "GUI/Button.h"
class GameMenu;

class UndoButton : public Button
{
public:
	UndoButton(sf::Vector2f buttonSize, sf::Color buttonColor, const sf::String& buttonText, const sf::Font& font, int fontSize, GameMenu& menu);

	void onClick() override;

	void onRelease() override;

	void onUnclick() override;
private:
	GameMenu& menu;
};