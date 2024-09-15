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
	constexpr float pieceScale{ .75f };
	constexpr float pieceSize{ pieceScale * GUI::pixelsPerTile };
	constexpr float offset{ (GUI::pixelsPerTile - pieceSize) / 2 };

	pieceTexture = texture;
	pieceSprite.setTexture(*pieceTexture, true);
	pieceSprite.setScale(pieceSize / pieceTexture->getSize().x, pieceSize / pieceTexture->getSize().y);

	pieceSprite.setPosition(getPosition() + sf::Vector2f{offset, offset});
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