#pragma once

#include "Piece/PieceEnums.h"
#include "GUI/GUIMain.h"
#include "SFML/Graphics.hpp"
#include "GUI/Object.h"
#include <vector>

namespace GUI
{
	class Chessboard : public Object
	{
	public:
		Chessboard(const std::string& objectName, const PieceTextures& pieceTextures)
			: Object{ objectName }, textures{ pieceTextures }
		{}

	protected:
		virtual void draw_impl(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Texture boardTex{};
		sf::Sprite boardSprite{};
		const PieceTextures& textures{};

		/**
		*	Sets a board tile to display a sprite
		*	@param texture Texture to display on this tile
		*	@param scale The percentage of the tile that this sprite's bounding box should take up
		*/
		void setTile(int idx, const sf::Texture& texture, float scale);

	public:
		void setBoardTexture(const sf::Texture& tex);
	};
}