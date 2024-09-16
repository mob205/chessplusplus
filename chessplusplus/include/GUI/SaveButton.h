#pragma once
#include "GUI/Button.h"

class GameMenu;
class SaveButton : public Button
{
public:
	SaveButton(sf::Vector2f buttonSize, sf::Color buttonColor, const sf::String& buttonText, const sf::Font& font, int fontSize, GameMenu& menu);

	void onClick() override;
	void onRelease() override;
	void onUnclick() override;
private:
	GameMenu& menu;
};