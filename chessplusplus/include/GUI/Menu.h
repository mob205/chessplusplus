#pragma once

#include "SFML/Graphics.hpp"
#include "GUI/Button.h"

class Menu : public sf::Drawable, public sf::Transformable
{
public:
	Menu(sf::Vector2f size);

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	virtual void onActive();

	void addButton(std::unique_ptr<Button> button, bool addToTexture = true);

	void addElement(const sf::Drawable& elem);

	virtual bool onButtonPress(sf::Vector2f point);

	virtual void onButtonRelease(sf::Vector2f point);

	virtual void onResize(sf::Vector2f center);

	virtual void onType(sf::Uint32 unicode);
protected:
	std::vector<std::unique_ptr<Button>> buttons{};

	sf::RenderTexture menuTexture{};
	sf::Sprite menuSprite{};

	void updateSprite();
};