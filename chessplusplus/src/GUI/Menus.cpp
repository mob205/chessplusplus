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

	// Helper function to calculate the X positions to layout buttons to fit evenly in a horizontal space
	template<int N>
	static inline consteval std::array<float, N> getButtonLayout(float buttonWidth, float totalWidth)
	{
		float leftPadding = (totalWidth - (N * buttonWidth)) / (N + 1);

		std::array<float, N> res{};
		for (int i = 0; i < N; ++i)
		{
			res[i] = ((i + 1) * leftPadding) + (buttonWidth / 2.f) + (i * buttonWidth);
		}
		return res;
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

		auto startWhite = makeButton("Start as White", font, { 350, 75 }, "White vs AI");
		startWhite->setPosition({ startSizeX / 2, startSizeY / 3 });
		startWhite->setInteractEvent([&]() { controller.startGame(PieceEnums::White); });
		mainMenu->addChild(startWhite);

		auto startBlack = makeButton("Start as White", font, { 350, 75 }, "Black vs AI");
		startBlack->setPosition({ startSizeX / 2, 2 * startSizeY / 3 });
		startBlack->setInteractEvent([&]() { controller.startGame(PieceEnums::Black); });
		mainMenu->addChild(startBlack);

		controller.setMainMenu(mainMenu);

		return mainMenu;
	}

	static std::shared_ptr<Object> makePromotionButton(const sf::Texture& texture, sf::Vector2f promoButtonSize)
	{

		auto button = std::make_shared<Object>("Promotion Button");

		auto buttonBackground = makeWrapper<sf::RectangleShape>("Promo Button Background", promoButtonSize);
		buttonBackground->getObject().setFillColor({ 100, 100, 100 });
		button->addChild(buttonBackground);

		sf::Vector2u textureSize = texture.getSize();
		sf::Vector2f scale{ promoButtonSize.x / textureSize.x, promoButtonSize.y / textureSize.y };

		auto buttonSprite = makeWrapper<sf::Sprite>("Button Sprite");
		buttonSprite->getObject().setTexture(texture, true);
		buttonSprite->setScale(scale);
		button->addChild(buttonSprite);

		// Center button elements horizontally
		buttonBackground->setPosition({ -promoButtonSize.x / 2.f, 0 });
		buttonSprite->setPosition({-promoButtonSize.x / 2.f, 0 });

		return button;
	}

	static std::shared_ptr<Object> makePromoMenu(const sf::Font& font, const PieceTextures& textures, GUIController& controller)
	{
		auto menu = std::make_shared<Object>("Promotion Menu");

		constexpr float buttonTopPadding{ 55 };
		constexpr float buttonLeftPadding{ 100 };
		constexpr float backgroundX{ 500 };
		constexpr float promoButtonSizeX{ 65 };

		const sf::Vector2f backgroundSize{ backgroundX, 150 };
		const sf::Vector2f promoButtonSize{ promoButtonSizeX, 65 };

		// Create menu background panel
		auto background = makeWrapper<sf::RectangleShape>("Promotion Background", backgroundSize);
		background->setInteractable(false);
		background->getObject().setFillColor({ 150, 150, 100 });
		menu->addChild(background);

		// Add text
		auto text = std::make_shared<TextObject>("Promotion Text", "Select a piece to promote to:", font, 30);
		sf::Vector2f textSize = text->getTextSize();
		
		// Center text
		text->setPosition({ (500 - textSize.x) / 2, 0 });
		menu->addChild(text);

		auto layoutX = getButtonLayout<4>(promoButtonSizeX, backgroundX);

		// Create and layout buttons
		auto knightButton = makePromotionButton(textures[PieceEnums::White][PieceEnums::Knight], promoButtonSize);
		knightButton->setInteractEvent([&]() { controller.onSelectPromotion('N'); });
		knightButton->setPosition({ layoutX[0], buttonTopPadding});
		menu->addChild(knightButton);

		auto rookButton = makePromotionButton(textures[PieceEnums::White][PieceEnums::Rook], promoButtonSize);
		rookButton->setInteractEvent([&]() { controller.onSelectPromotion('R'); });
		rookButton->setPosition({ layoutX[1], buttonTopPadding });
		menu->addChild(rookButton);

		auto bishopButton = makePromotionButton(textures[PieceEnums::White][PieceEnums::Bishop], promoButtonSize);
		bishopButton->setInteractEvent([&]() { controller.onSelectPromotion('B'); });
		bishopButton->setPosition({ layoutX[2], buttonTopPadding});
		menu->addChild(bishopButton);

		auto queenButton = makePromotionButton(textures[PieceEnums::White][PieceEnums::Queen], promoButtonSize);
		queenButton->setInteractEvent([&]() { controller.onSelectPromotion('Q'); });
		queenButton->setPosition({ layoutX[3], buttonTopPadding});
		menu->addChild(queenButton);

		return menu;
	}

	std::shared_ptr<Object> createChessMenu(const sf::Font& font, GUIController& controller, const PieceTextures& textures)
	{
		constexpr float buttonsTopPadding{ (startSizeY / 32.f) + (buttonHeight / 2.f) };

		const sf::Vector2f buttonSize = { buttonWidth, buttonHeight };

		constexpr float centerX = startSizeX / 2.f;
		constexpr float centerY = startSizeY / 2.f;

		auto menu = std::make_shared<Object>("Chess Menu");


		auto layoutX = getButtonLayout<4>(buttonWidth, startSizeX);

		// Create top buttons
		auto quitButton = makeButton("Quit Button", font, buttonSize, "Quit");
		quitButton->setPosition({ layoutX[0], buttonsTopPadding});
		quitButton->setInteractEvent([&]() { controller.onQuit(); });
		menu->addChild(quitButton);

		auto undoButton = makeButton("Undo Button", font, buttonSize, "Undo");
		undoButton->setPosition({ layoutX[1], buttonsTopPadding});
		undoButton->setInteractEvent([&]() { controller.onUndo(); });
		menu->addChild(undoButton);

		auto loadButton = makeButton("Load Button", font, buttonSize, "Load");
		loadButton->setPosition({ layoutX[2], buttonsTopPadding});
		loadButton->setInteractEvent([&]() { controller.onLoad(); });
		menu->addChild(loadButton);

		auto saveButton = makeButton("Save Button", font, buttonSize, "Save");
		saveButton->setPosition({ layoutX[3], buttonsTopPadding});
		saveButton->setInteractEvent([&]() { controller.onSave(); });
		menu->addChild(saveButton);

		// Create chess board
		auto board = std::make_shared<Chessboard>("Chess Board", textures);
		board->setPosition({ centerX - 150, centerY });
		menu->addChild(board);

		// Create GUI controller components
		auto turnCounterCenter = std::make_shared<Object>();
		turnCounterCenter->setPosition({ centerX - 150, 2 * buttonsTopPadding });
		menu->addChild(turnCounterCenter);

		auto turnCounter = std::make_shared<TextObject>("Turn Counter", "", font, 30);
		turnCounterCenter->addChild(turnCounter);

		auto turnLog = std::make_shared<TurnLog>(18, font);
		turnLog->setPosition(centerX + 150, 185);
		menu->addChild(turnLog);

		auto promotionMenu = makePromoMenu(font, textures, controller);
		promotionMenu->setPosition(centerX / 2, startSizeY - 175);
		menu->addChild(promotionMenu);

		auto saveTextBox = std::make_shared<Object>("Save Text Box");
		saveTextBox->setPosition({600, 125});
		menu->addChild(saveTextBox);

		auto saveBoxBackground = makeWrapper<sf::RectangleShape>("Save Box Background", sf::Vector2f{ 400, 35 });
		saveBoxBackground->getObject().setFillColor({ 100, 100, 100 });
		saveTextBox->addChild(saveBoxBackground);

		auto saveText = std::make_shared<TextObject>("Save Text", "", font, 30);
		saveText->setPosition({ 10, -2 });
		saveTextBox->addChild(saveText);

		// Set dependencies
		controller.setBoard(board);
		controller.setGameMenu(menu);
		controller.setTurnCounter(turnCounter);
		controller.setTurnLog(turnLog);
		controller.setPromotionMenu(promotionMenu);
		controller.setSaveBox(saveText);

		return menu;

	}
}