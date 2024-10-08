#pragma once
#include "GUI/Menu.h"
#include "GUI/TileButton.h"
#include "GUI/PromoButton.h"
#include "Game/Game.h"

class GameMenu : public Menu
{
public:
	GameMenu(sf::Vector2f size, std::vector<std::vector<sf::Texture>>& pieceTextures);

	void addTileButton(std::unique_ptr<TileButton> button);

	void addPromoButton(std::unique_ptr<PromoButton> button);

	void onActive() override;

	void onResize(sf::Vector2f center) override;

	bool onButtonPress(sf::Vector2f point) override;

	void onType(sf::Uint32 unicode) override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setLogText(std::unique_ptr<sf::Text> log);

	void setSaveText(std::unique_ptr<sf::Text> saveFileText);

	void setTurnCounter(std::unique_ptr<sf::Text> turnCounter);

	void insertPromoInput(char input);

	void undo();

	void saveGame();
	void loadGame();

	sf::RenderTexture& getPromoTexture();

private:
	void updateBoard();
	void updateTurnCounter();
	void updatePromo();

	void selectPiece(int i);
	void unselectPiece(bool clearLog);
	void logMove(MoveResult res);

	int selectedPiece{ -1 };

	bool isGameDone{};

	bool isPromoting{};
	Point promoStart{};
	Point promoEnd{};


	std::unique_ptr<sf::Text> eventLog{};
	sf::Vector2f logOffset{};

	std::unique_ptr<sf::Text> turnCounter{};
	sf::Vector2f counterOffset{};

	std::vector<std::vector<sf::Texture>>& pieceTextures;

	std::vector<std::unique_ptr<TileButton>> tiles{};
	std::unique_ptr<Game> game{};

	sf::RenderTexture promotionTexture{};
	sf::Sprite promoSprite{};
	sf::Vector2f promotionOffset{};
	sf::Vector2f promoInitPos{ 750, 750 };

	std::vector<std::unique_ptr<PromoButton>> promoButtons{};

	std::unique_ptr<sf::Text> saveFileText{};
	sf::Vector2f saveFileOffset{};
	std::string currentSaveFile{};
};