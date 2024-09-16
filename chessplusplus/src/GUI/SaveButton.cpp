#include "GUI/SaveButton.h"
#include "GUI/GameMenu.h"

SaveButton::SaveButton(sf::Vector2f buttonSize, sf::Color buttonColor, const sf::String& buttonText, const sf::Font& font, int fontSize, GameMenu& menu)
	: Button{ buttonSize, buttonColor, buttonText, font, fontSize }, menu{ menu }
{}


void SaveButton::onClick()
{
	menu.saveGame();
}

void SaveButton::onRelease()
{
}

void SaveButton::onUnclick()
{
}
