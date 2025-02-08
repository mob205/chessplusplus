#include "GUI/Chessboard.h"
#include "GUI/SFMLObject.h"
#include "Piece/PieceEnums.h"
#include "Board/Board.h"
#include "Game/Settings.h"
#include "Game/Point.h"
#include <memory>
#include <iostream>

namespace GUI
{
	const sf::Color highlightColor{ 0, 252, 0, 150 };

	Chessboard::Chessboard(const std::string& objectName, const PieceTextures& pieceTextures)
		: Object{ objectName }, textures{ pieceTextures }
	{
		sf::RenderTexture boardTexture{};
		boardTexture.create(boardLength, boardLength);

		const sf::Vector2f tileSize{ pixelsPerTile, pixelsPerTile };
		const sf::Vector2f interactOffset{ -boardLength / 2.f, -boardLength / 2.f };

		float offsetX{ 0 };
		float offsetY{ pixelsPerTile * (Settings::boardSize - 1) };
		for (int i = 0; i < Settings::boardSize; ++i)
		{
			float curOffsetX{ offsetX };

			for (int j = 0; j < Settings::boardSize; ++j)
			{
				// Name for debug purposes
				std::string name = std::string{ "Tile " } + std::string(1, j + 'a') + std::to_string(i + 1);

				sf::Vector2f tilePosition{ curOffsetX, offsetY };

				// Setup a square to draw the board texture
				sf::RectangleShape boardSquare{ tileSize };
				boardSquare.setPosition(tilePosition);
				sf::Color color = (i + j) % 2 == 0 ? teamBlackColor : teamWhiteColor;
				boardSquare.setFillColor(color);
				boardTexture.draw(boardSquare);

				// Setup a rect that is clickable
				auto tileInteractable = makeWrapper<sf::RectangleShape>(name, tileSize);
				tileInteractable->setPosition(tilePosition + interactOffset);

				// For tile highlighting
				tileInteractable->getObject().setFillColor(highlightColor);

				tileInteractable->setVisibility(false);
				tileInteractable->setInteractEvent(
					[=]() {
						interactTile({i, j});
					});
				addChild(tileInteractable);

				// Setup a sprite for piece sprites
				auto tileSprite = makeWrapper<sf::Sprite>("Tile Sprite");
				tileInteractable->addChild(tileSprite);
				tileSprite->setInteractable(false);

				// Fill columns left to right
				curOffsetX += pixelsPerTile;
			}
			// Fill rows bottom to top
			offsetY -= pixelsPerTile;
		}
		setBoardTexture(boardTexture.getTexture());
	}
	void Chessboard::draw_impl(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(boardSprite, states);
	}
	void Chessboard::setTile(int idx, const sf::Texture& texture, float scaleFactor)
	{
		// Child hierarchy is Chessboard -> Rect for interaction -> Sprite
		auto sprite = std::static_pointer_cast<SFMLObject<sf::Sprite>>(children[idx]->getChild(0));
		sprite->getObject().setTexture(texture, true);

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

	void Chessboard::highlightTile(Point pos, bool isHighlighted)
	{
		children[pos.file + Settings::boardSize * pos.rank]->setVisibility(isHighlighted);
	}

	void Chessboard::unhighlightAllTiles()
	{
		for (auto child : children)
		{
			child->setVisibility(false);
		}
	}


	void Chessboard::interactTile(Point pos)
	{
		if (onTileInteractedEvent)
		{
			onTileInteractedEvent(pos);
		}
	}

	void Chessboard::updateBoard(const Board& gameBoard)
	{
		for (int i = 0; i < Settings::boardSize; ++i)
		{
			for (int j = 0; j < Settings::boardSize; ++j)
			{
				int childIndex = i * Settings::boardSize + j;
				const Piece* piece = gameBoard[{i, j}].get();
				
				const sf::Texture* tex{};
				if (piece)
				{
					tex = &textures[piece->getTeam()][piece->getType()];
				}
				else
				{
					tex = &textures[PieceEnums::White][PieceEnums::None];
				}

				setTile(childIndex, *tex, .8f);
			}
		}
	}

}