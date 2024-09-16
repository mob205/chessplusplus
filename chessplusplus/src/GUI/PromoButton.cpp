#include "GUI/PromoButton.h"
#include "GUI/GameMenu.h"
#include "GUI/GUIMain.h"

PromoButton::PromoButton(sf::Vector2f buttonSize, sf::Color buttonColor, sf::Texture& pieceTexture, GameMenu& menu, char input)
	: Button{ buttonSize, buttonColor }, buttonSize{ buttonSize }, pieceTexture { pieceTexture }, menu{ menu }, input{ input }
{}

void PromoButton::onClick()
{
	menu.insertPromoInput(input);
}

void PromoButton::onRelease()
{
}

void PromoButton::onUnclick()
{
}

void PromoButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Button::draw(target, states);
	sf::Sprite sprite{ pieceTexture };
	states.transform *= getTransform();

	constexpr float pieceScale{ .65f };
	constexpr float pieceSize{ pieceScale * GUI::pixelsPerTile };
	constexpr float offset{ (GUI::pixelsPerTile - pieceSize) / 2 };

	sprite.setPosition(getPosition());
	sprite.setScale(pieceSize / pieceTexture.getSize().x, pieceSize / pieceTexture.getSize().y);

	target.draw(sprite);
}

void PromoButton::setOffset(sf::Vector2f offset)
{
	positionOffset = offset;
}

void PromoButton::recenter(sf::Vector2f center)
{
	setPosition(center + positionOffset);
}