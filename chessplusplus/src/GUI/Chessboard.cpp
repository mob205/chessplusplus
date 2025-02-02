#include "GUI/Chessboard.h"
#include <iostream>

namespace GUI
{
	void Chessboard::draw_impl(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(boardSprite, states);
	}
	void Chessboard::setBoardTexture(const sf::Texture& tex)
	{
		boardTex = tex;
		boardSprite.setTexture(boardTex); 
		boardSprite.setOrigin({ static_cast<float>(boardTex.getSize().x) / 2.f, static_cast<float>(boardTex.getSize().y) / 2.f });
	}
}