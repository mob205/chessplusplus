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

void Menu::addButton(std::unique_ptr<Button> button, bool addToTexture)
{
	buttons.emplace_back(std::move(button));

	if (addToTexture)
	{
		addElement(*buttons.back());
	}
}

void Menu::addElement(const sf::Drawable& elem)
{
	menuTexture.draw(elem);
	updateSprite();
}

bool Menu::onButtonPress(sf::Vector2f point)
{
	for (int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i]->containsPoint(point - getPosition()))
		{
			std::cout << "Clicked on button " << i << "\n";
			buttons[i]->onClick();
			return true;
		}
	}
	return false;
}

void Menu::onButtonRelease(sf::Vector2f point)
{
	//std::cout << "Event release at relative position (" << point.x - getPosition().x << ", " << point.y - getPosition().y << ")\n";
	for (int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i]->containsPoint(point - getPosition()))
		{
			std::cout << "Released on button " << i << "\n";
			buttons[i]->onRelease();
			break;
		}
	}
}

void Menu::updateSprite()
{
	menuTexture.display();
	menuSprite.setTexture(menuTexture.getTexture());
}

void Menu::onActive()
{

}

void Menu::onResize(sf::Vector2f center)
{

}

void Menu::onType(sf::Uint32 unicode)
{

}