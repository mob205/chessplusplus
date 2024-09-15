#include "GUI/TileButton.h"
#include "GUI/GUIMain.h"

void TileButton::onClick()
{
}

void TileButton::onRelease()
{
}

void TileButton::onUnclick()
{

}

void TileButton::setPieceSprite(sf::Texture* texture)
{
	pieceTexture = texture;
	pieceSprite.setTexture(*pieceTexture, true);
	pieceSprite.setScale(static_cast<float>(GUI::pixelsPerTile) / pieceTexture->getSize().x, static_cast<float>(GUI::pixelsPerTile) / pieceTexture->getSize().y);
	pieceSprite.setPosition(getPosition());
}

void TileButton::resetPieceSprite()
{
	pieceTexture = nullptr;
}

void TileButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Button::draw(target, states);
	if (pieceTexture)
	{
		target.draw(pieceSprite, states);
	}
}

void TileButton::setOffset(sf::Vector2f offset)
{
	positionOffset = offset;
}

void TileButton::recenter(sf::Vector2f center)
{
	setPosition(center + positionOffset);
	pieceSprite.setPosition(getPosition());
}