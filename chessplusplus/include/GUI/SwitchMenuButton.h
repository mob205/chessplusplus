#pragma once
#include "SFML/Graphics.hpp"
#include "GUI/MenuManager.h"
#include "GUI/Button.h"

class SwitchMenuButton : public Button
{
public:
	SwitchMenuButton(sf::Vector2f buttonSize, sf::Color buttonColor, const sf::String& buttonText, const sf::Font& font, int fontSize, MenuManager& menuManager, int menuIndex)
		: Button{ buttonSize, buttonColor, buttonText, font, fontSize }, menuManager{menuManager}, menuIndex{menuIndex}
	{}

	void onClick() override;
	void onRelease() override;
	void onUnclick() override;

private:
	MenuManager& menuManager;
	int menuIndex{};
};