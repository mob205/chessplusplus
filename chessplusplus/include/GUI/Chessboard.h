#pragma once

#include "SFML/Graphics.hpp"
#include "GUI/Object.h"
#include <vector>

namespace GUI
{
	class Chessboard : public Object
	{
	public:
		Chessboard(const std::string& objectName)
			: Object{ objectName }
		{}

	protected:
		virtual void draw_impl(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Texture boardTex;
		sf::Sprite boardSprite{};

	public:
		void setBoardTexture(const sf::Texture& tex);
	};
}