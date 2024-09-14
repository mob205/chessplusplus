#pragma once
#include "SFML/Graphics.hpp"
#include "GUI/Button.h"

class TestButton : public Button
{
public:
	TestButton(sf::Vector2f buttonSize, sf::Color buttonColor, const sf::String& buttonText, const sf::Font& font, int fontSize)
		: Button{ buttonSize, buttonColor, buttonText, font, fontSize }
	{}
	void onClick() override;
	void onRelease() override;
	void onUnclick() override;
};