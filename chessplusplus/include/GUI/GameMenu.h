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

	void onResize(sf::Vector2f center) override;

	void onButtonPress(sf::Vector2f point) override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setLogText(std::unique_ptr<sf::Text> log);

	void setTurnCounter(std::unique_ptr<sf::Text> turnCounter);

private:
	void updateBoard();
	void updateTurnCounter();

	void selectPiece(int i);
	void unselectPiece(bool clearLog);
	void logMove(MoveResult res);

	int selectedPiece{-1};

	std::unique_ptr<sf::Text> eventLog{};
	sf::Vector2f logOffset{};

	std::unique_ptr<sf::Text> turnCounter{};
	sf::Vector2f counterOffset{};

	std::vector<std::vector<sf::Texture>>& pieceTextures;

	std::vector<std::unique_ptr<TileButton>> tiles{};
	std::unique_ptr<Game> game{};

};