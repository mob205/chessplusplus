#include "GUI/Menu.h"
#include "SFML/Graphics.hpp"
#include <iostream>

Menu::Menu(sf::Vector2f size)
{
	menuTexture.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y));
	updateSprite();
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(menuSprite, states);
}

void Menu::addButton(std::unique_ptr<Button> button)
{
	buttons.emplace_back(std::move(button));
	addElement(*buttons.back());
}

void Menu::addElement(const sf::Drawable& elem)
{
	menuTexture.draw(elem);
	updateSprite();
}

void Menu::onButtonPress(sf::Vector2f point)
{
	std::cout << "Event click at relative position (" << point.x - getPosition().x << ", " << point.y - getPosition().y << ")\n";
	for (int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i]->containsPoint(point - getPosition()))
		{
			std::cout << "Clicked on button " << i << "\n";
			buttons[i]->onClick();
			break;
		}
	}
}

void Menu::onButtonRelease(sf::Vector2f point)
{
	
	for (int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i]->containsPoint(point - getPosition()))
		{
			buttons[i]->onRelease();
		}
	}
}

void Menu::updateSprite()
{
	menuTexture.display();
	menuSprite.setTexture(menuTexture.getTexture());
}
