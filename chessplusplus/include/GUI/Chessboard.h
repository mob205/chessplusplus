#pragma once

#include "Piece/PieceEnums.h"
#include "GUI/GUIMain.h"
#include "SFML/Graphics.hpp"
#include "GUI/Object.h"
#include "Game/Point.h"
#include <vector>

class Board;

namespace GUI
{
	class Chessboard : public Object
	{
	public:
		Chessboard(const std::string& objectName, const PieceTextures& pieceTextures);

		void highlightTile(Point pos, bool isHighlighted);
		void unhighlightAllTiles();

		void updateBoard(const Board& board);

	protected:
		virtual void draw_impl(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Texture boardTex{};
		sf::Sprite boardSprite{};
		const PieceTextures& textures{};

		PieceEnums::Team currentTeamOrientation;

		std::function<void(Point)> onTileInteractedEvent;

		/**
		*	Sets a board tile to display a sprite
		*	@param texture Texture to display on this tile
		*	@param scale The percentage of the tile that this sprite's bounding box should take up
		*/
		void setTile(int idx, const sf::Texture& texture, float scale);

		void setBoardTexture(const sf::Texture& tex);

		void interactTile(Point pos);
		
		float getTeamRotation() const;

	public:

		// Displays the board from the team side's perspective
		void setOrientation(PieceEnums::Team team) { currentTeamOrientation = team; }

		void setOnTileInteracted(std::function<void(Point)> event) { onTileInteractedEvent = event; }
	};
}