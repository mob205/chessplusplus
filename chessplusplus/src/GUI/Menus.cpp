#include "SFML/Graphics.hpp"
#include "GUI/Menus.h"
#include "GUI/GUIController.h"
#include "GUI/SFMLObject.h"
#include "GUI/TextObject.h"
#include "GUI/GUIMain.h"
#include "GUI/Chessboard.h"
#include "GUI/TurnLog.h"
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

	std::shared_ptr<Object> createChessMenu(const sf::Font& font, GUIController& controller, const PieceTextures& textures)
	{
		constexpr float buttonsTopPadding{ (startSizeY / 32.f) + (buttonHeight / 2.f) };

		const sf::Vector2f buttonSize = { buttonWidth, buttonHeight };

		constexpr float centerX = startSizeX / 2.f;
		constexpr float centerY = startSizeY / 2.f;

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

		auto board = std::make_shared<Chessboard>("Chess Board", textures);
		board->setPosition({ centerX - 150, centerY });
		menu->addChild(board);

		auto turnCounterCenter = std::make_shared<Object>();
		turnCounterCenter->setPosition({ centerX - 150, 2 * buttonsTopPadding });
		menu->addChild(turnCounterCenter);

		auto turnCounter = std::make_shared<TextObject>("Turn Counter", "", font, 30);
		turnCounterCenter->addChild(turnCounter);

		auto turnLog = std::make_shared<TurnLog>(18, font);
		turnLog->setPosition(centerX + 150, 185);
		menu->addChild(turnLog);

		controller.setBoard(board);
		controller.setGameMenu(menu);
		controller.setTurnCounter(turnCounter);
		controller.setTurnLog(turnLog);

		return menu;

	}
}