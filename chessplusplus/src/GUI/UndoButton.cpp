#include "SFML/Graphics.hpp"
#include "GUI/UndoButton.h"
#include "GUI/GameMenu.h"

UndoButton::UndoButton(sf::Vector2f buttonSize, sf::Color buttonColor, const sf::String& buttonText, const sf::Font& font, int fontSize, GameMenu& menu)
	: Button{ buttonSize, buttonColor, buttonText, font, fontSize }, menu{ menu }
{}

void UndoButton::onClick()
{
	menu.undo();
}

void UndoButton::onRelease()
{
}

void UndoButton::onUnclick()
{
}
