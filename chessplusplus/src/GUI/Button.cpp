#include "SFML/Graphics.hpp"
#include "GUI/Button.h"
#include <iostream>

Button::Button(sf::Vector2f buttonSize, sf::Color buttonColor, const sf::String& buttonText, const sf::Font& font, int fontSize)
{
	buttonTexture.create(static_cast<unsigned int>(buttonSize.x), static_cast<unsigned int>(buttonSize.y));

	sf::RectangleShape buttonRect{ buttonSize };
    buttonRect.setFillColor(buttonColor);
    buttonTexture.draw(buttonRect);

    sf::Text rectText{ buttonText, font, static_cast<unsigned int>(fontSize)};
    rectText.setFillColor(sf::Color::Black);
    rectText.setPosition((buttonSize.x - rectText.getGlobalBounds().width)/ 2, (buttonSize.y / 2) - rectText.getGlobalBounds().height);
    buttonTexture.draw(rectText);

    buttonTexture.display();
    buttonSprite.setTexture(buttonTexture.getTexture());
}

bool Button::containsPoint(sf::Vector2f point)
{
    return buttonSprite.getGlobalBounds().contains(point - this->getPosition());
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
	target.draw(buttonSprite, states);
}
