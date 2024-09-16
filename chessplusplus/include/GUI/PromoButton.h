#pragma once
#include "Button.h"

class GameMenu;

class PromoButton : public Button
{
public:
	PromoButton(sf::Vector2f buttonSize, sf::Color buttonColor, sf::Texture& pieceTexture, GameMenu& menu, char input);

	void onClick() override;
	void onRelease() override;
	void onUnclick() override;

	void setOffset(sf::Vector2f offset);
	void recenter(sf::Vector2f center);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	GameMenu& menu;
	char input{};
	sf::Vector2f buttonSize{};
	sf::Vector2f positionOffset{};
	sf::Texture& pieceTexture;
};