#include "SFML/Graphics.hpp"
#include "GUI/Menus.h"
#include "GUI/GUIController.h"
#include "GUI/SFMLObject.h"
#include "GUI/TextObject.h"
#include "GUI/GUIMain.h"
#include "GUI/Chessboard.h"
#include "Game/Settings.h"

namespace GUI
{
	// Helper function to make an object with centered rectangle and centered text
	static std::shared_ptr<Object> makeButton(const std::string& buttonName, const sf::Font& font, sf::Vector2f size, const std::string& text)
	{
		auto button = std::make_shared<Object>(buttonName);

		auto buttonRect = makeWrapper<sf::RectangleShape>(buttonName + " Background", size);
		buttonRect->setPosition(-size / 2.f);
		buttonRect->getObject().setFillColor(buttonColor);
		button->addChild(buttonRect);

		auto buttonText = std::make_shared<TextObject>(buttonName + " Text", text, font, 50);
		sf::Vector2f textSize = buttonText->getTextSize();
		buttonText->setPosition({ -textSize.x / 2, -textSize.y });
		button->addChild(buttonText);

		return button;
	}

	// Helper function to get the desired X position for a chess menu button
	static inline consteval float getButtonX(int buttonNumber)
	{
		constexpr float buttonLeftPadding{ startSizeX / 64.f };

		return ((buttonNumber + 1) * buttonLeftPadding) + (buttonWidth / 2.f) + (buttonNumber * buttonWidth);
	}

	static std::shared_ptr<Object> makeChessboard()
	{
		//const sf::Vector2f center{ startSizeX / 2, startSizeY / 2 };
		const sf::Vector2f tileSize{ pixelsPerTile, pixelsPerTile };

		auto board = std::make_shared<Chessboard>("Chess Board");
		board->setChildrenVisibility(false);

		sf::RenderTexture boardTexture{};
		boardTexture.create(boardLength, boardLength);

		sf::Vector2f interactOffset{ -boardLength / 2.f, -boardLength / 2.f };

		float offsetX{ 0 };
		float offsetY{ 0 };
		for(int i = 0; i < Settings::boardSize; ++i)
		{
			float curOffsetX{ offsetX };

			for (int j = 0; j < Settings::boardSize; ++j)
			{
				sf::Color color = (i + j) % 2 == 0 ? teamBlackColor : teamWhiteColor;
				
				// Name for debug purposes
				std::string name = std::string{ "Tile Row: " } + std::to_string(i) + std::string{ " Col: " } + std::to_string(j);

				// Set up tile
				auto tile = makeWrapper<sf::RectangleShape>(name, sf::Vector2f{tileSize});
				board->addChild(tile);
				tile->setPosition({ curOffsetX, offsetY });
				tile->getObject().setFillColor(color);

				// Draw to render texture once rather than drawing individual tiles every frame
				boardTexture.draw(*tile);
				tile->setVisibility(false);

				// Move tile into place for interacting. Render textures don't support negative coordinates, so need to do it after drawing
				tile->move(interactOffset);

				// Fill columns left to right
				curOffsetX += pixelsPerTile;
			}
			// Fill rows top to bottom
			offsetY += pixelsPerTile;
		}
		
		board->setBoardTexture(boardTexture.getTexture());
		return board;
	}

	std::shared_ptr<Object> createMainMenu(const sf::Font& font, GUIController& controller)
	{
		auto mainMenu = std::make_shared<Object>("Main Menu");

		auto welcomeTextCenter = std::make_shared<Object>("Welcome Text");
		welcomeTextCenter->setPosition({ startSizeX / 2, 150 });
		mainMenu->addChild(welcomeTextCenter);

		auto welcomeText = std::make_shared<TextObject>("Welcome to Chess!", font, 50);
		sf::Vector2f textSize = welcomeText->getTextSize();
		welcomeText->setPosition(-textSize / 2.f);
		welcomeTextCenter->addChild(welcomeText);

		auto startButton = makeButton("Start Button", font, { 250, 75 }, "PLAY");
		startButton->setPosition({ startSizeX / 2, startSizeY / 2 });
		startButton->setInteractEvent([&]() { controller.onStart(); });
		mainMenu->addChild(startButton);

		controller.setMainMenu(mainMenu);

		return mainMenu;
	}

	std::shared_ptr<Object> createChessMenu(const sf::Font& font, GUIController& controller)
	{
		constexpr float buttonsTopPadding{ (startSizeX / 64.f) + (buttonHeight / 2.f) };

		const sf::Vector2f buttonSize = { buttonWidth, buttonHeight };

		auto menu = std::make_shared<Object>("Chess Menu");

		auto quitButton = makeButton("Quit Button", font, buttonSize, "Quit");
		quitButton->setPosition({ getButtonX(0) , buttonsTopPadding});
		quitButton->setInteractEvent([&]() { controller.onQuit(); });
		menu->addChild(quitButton);

		auto undoButton = makeButton("Undo Button", font, buttonSize, "Undo");
		undoButton->setPosition({ getButtonX(1), buttonsTopPadding});
		undoButton->setInteractEvent([&]() { controller.onUndo(); });
		menu->addChild(undoButton);

		auto loadButton = makeButton("Load Button", font, buttonSize, "Load");
		loadButton->setPosition({ getButtonX(2), buttonsTopPadding});
		loadButton->setInteractEvent([&]() { controller.onLoad(); });
		menu->addChild(loadButton);

		auto saveButton = makeButton("Save Button", font, buttonSize, "Save");
		saveButton->setPosition({ getButtonX(3), buttonsTopPadding});
		saveButton->setInteractEvent([&]() { controller.onSave(); });
		menu->addChild(saveButton);

		auto board = makeChessboard();
		board->setPosition({ startSizeX / 2, startSizeY / 2 });
		menu->addChild(board);

		controller.setGameMenu(menu);

		return menu;

	}
}