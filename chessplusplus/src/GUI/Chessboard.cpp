#include "GUI/Chessboard.h"
#include "GUI/SFMLObject.h"
#include "Piece/PieceEnums.h"
#include <memory>
#include <iostream>

namespace GUI
{
	void Chessboard::draw_impl(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(boardSprite, states);
	}
	void Chessboard::setTile(int idx, const sf::Texture& texture, float scaleFactor)
	{
		// Child hierarchy is Chessboard -> Rect for interaction -> Sprite
		auto sprite = std::static_pointer_cast<SFMLObject<sf::Sprite>>(children[idx]->getChild(0));
		sprite->getObject().setTexture(texture);

		sf::Vector2u texSize = texture.getSize();
		if (texSize == sf::Vector2u{ 0, 0 }) { return; }
		
		sf::Vector2f scale = { pixelsPerTile / static_cast<float>(texSize.x),
							   pixelsPerTile / static_cast<float>(texSize.y) };

		// Scale and center sprite
		sprite->setScale(scaleFactor * scale);
		sprite->setPosition(sf::Vector2f{ pixelsPerTile, pixelsPerTile } * (1 - scaleFactor) * .5f);
	}
	void Chessboard::setBoardTexture(const sf::Texture& tex)
	{
		boardTex = tex;
		boardSprite.setTexture(boardTex); 
		boardSprite.setOrigin({ static_cast<float>(boardTex.getSize().x) / 2.f, static_cast<float>(boardTex.getSize().y) / 2.f });
	}

}