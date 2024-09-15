#pragma once
#include "GUI/Menu.h"
#include "GUI/TileButton.h"
#include "Game/Game.h"

class GameMenu : public Menu
{
public:
	GameMenu(sf::Vector2f size, std::vector<std::vector<sf::Texture>>& pieceTextures);

	void addTileButton(std::unique_ptr<TileButton> button);

	void onActive() override;

	void onButtonPress(sf::Vector2f point) override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::RenderTexture boardTexture{};
	sf::Sprite boardSprite{};

	std::vector<std::unique_ptr<TileButton>> tiles{};
	std::unique_ptr<Game> game{};
	std::vector<std::vector<sf::Texture>>& pieceTextures;

	void updateBoard();
};