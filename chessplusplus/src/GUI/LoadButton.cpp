#include "GUI/LoadButton.h"
#include "GUI/GameMenu.h"

LoadButton::LoadButton(sf::Vector2f buttonSize, sf::Color buttonColor, const sf::String& buttonText, const sf::Font& font, int fontSize, GameMenu& menu)
	: Button{ buttonSize, buttonColor, buttonText, font, fontSize }, menu{ menu }
{}


void LoadButton::onClick()
{
	menu.loadGame();
}

void LoadButton::onRelease()
{
}

void LoadButton::onUnclick()
{
}
