#pragma once
#include "SFML/Graphics.hpp"
#include "GUI/Button.h"


class TileButton : public Button
{
public:
	TileButton(sf::Vector2f buttonSize, sf::Color buttonColor)
		: Button{ buttonSize, buttonColor }
	{}
	void onClick() override;
	void onRelease() override;
	void onUnclick() override;

	void setPieceSprite(sf::Texture* pieceTexture);
	void resetPieceSprite();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Texture* pieceTexture{};
	sf::Sprite pieceSprite{};
};