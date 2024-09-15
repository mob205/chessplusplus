#include "GUI/GameMenu.h"
#include "SFML/Graphics.hpp"
#include "GUI/GUIMain.h"
#include "Game/Game.h"
#include "Board/Board.h"
#include "Game/Settings.h"

GameMenu::GameMenu(sf::Vector2f size, std::vector<std::vector<sf::Texture>>& pieceTextures)
	: Menu{ size }, pieceTextures{pieceTextures}
{
	boardTexture.create(GUI::boardLength, GUI::boardLength);
}

void GameMenu::addTileButton(std::unique_ptr<TileButton> tile)
{
	tiles.emplace_back(std::move(tile));
}

void GameMenu::onButtonPress(sf::Vector2f point)
{
	Menu::onButtonPress(point);
	for (int i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i]->containsPoint(point))
		{
			std::cout << "Click on tile " << i << '\n';
			break;
		}
	}
}

void GameMenu::onActive()
{
	game = std::make_unique<Game>();
	updateBoard();
}

void GameMenu::updateBoard()
{
	const Board& board{ game->getBoard() };
	for (int i = 0; i < Settings::boardSize; ++i)
	{
		for (int j = 0; j < Settings::boardSize; ++j)
		{
			int tileIdx = i * Settings::boardSize + j;
			Piece* piece{ board[{i, j}].get() };
			if (piece != nullptr)
			{ 
				tiles[tileIdx]->setPieceSprite(&pieceTextures[piece->getTeam()][piece->getType()]);
			}
		}
	}

}

void GameMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Menu::draw(target, states);
	
	for (int i = 0; i < tiles.size(); ++i)
	{
		target.draw(*tiles[i]);
	}
}

void GameMenu::onResize(sf::Vector2f center)
{
	for (int i = 0; i < tiles.size(); ++i)
	{
		tiles[i]->recenter(center);
	}
}