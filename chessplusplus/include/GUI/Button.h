#pragma once
#include "SFML/Graphics.hpp"

class Button : public sf::Drawable, public sf::Transformable
{
public:
	Button(sf::Vector2f buttonSize, sf::Color buttonColor);
	Button(sf::Vector2f buttonSize, sf::Color buttonColor, const sf::String& buttonText, const sf::Font& font, int fontSize);

	sf::Sprite& getSprite() { return buttonSprite; }

	bool containsPoint(sf::Vector2f point);

	// Called when the mouse button is pressed while hovering over the button
	virtual void onClick() = 0;

	// Called when the mouse button is released while hovering over the button
	virtual void onRelease() = 0;

	// Called when the mouse button is pressed while hovering off the button
	// after an onClick() call
	virtual void onUnclick() = 0;

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
private:
	sf::RenderTexture buttonTexture{};
	sf::Sprite buttonSprite{};
};