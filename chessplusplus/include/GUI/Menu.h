#pragma once

#include "SFML/Graphics.hpp"
#include "GUI/Button.h"

class Menu : public sf::Drawable, public sf::Transformable
{
public:
	Menu(sf::Vector2f size);

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	void addButton(std::unique_ptr<Button> button);

	void addElement(const sf::Drawable& elem);

	void onButtonPress(sf::Vector2f point);

	void onButtonRelease(sf::Vector2f point);
private:
	std::vector<std::unique_ptr<Button>> buttons{};

	sf::RenderTexture menuTexture{};
	sf::Sprite menuSprite{};

	void updateSprite();
};