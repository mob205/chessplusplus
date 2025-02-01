#include <iostream>
#include <memory>
#include "SFML/Graphics.hpp"

#include "Game/Settings.h"
#include "Game/Game.h"
#include "Piece/PieceEnums.h"

#include "GUI/GUISettings.h"
#include "GUI/Autoscaler.h"
#include "GUI/ShapeObject.h"
#include "GUI/TextObject.h"

namespace GUI
{
	const sf::Color buttonColor{ 215, 215, 215 };

	static std::vector<std::vector<sf::Texture>> pieceTextures(PieceEnums::MaxTeams, std::vector<sf::Texture>(PieceEnums::MaxTypes));
	static sf::Font font;

	static bool loadPieceTextures()
	{
		bool res{ true };
		res &= pieceTextures[PieceEnums::White][PieceEnums::Pawn].loadFromFile("../Resources/w_pawn.png");
		res &= pieceTextures[PieceEnums::White][PieceEnums::Knight].loadFromFile("../Resources/w_knight.png");
		res &= pieceTextures[PieceEnums::White][PieceEnums::Rook].loadFromFile("../Resources/w_rook.png");
		res &= pieceTextures[PieceEnums::White][PieceEnums::Queen].loadFromFile("../Resources/w_queen.png");
		res &= pieceTextures[PieceEnums::White][PieceEnums::King].loadFromFile("../Resources/w_king.png");
		res &= pieceTextures[PieceEnums::White][PieceEnums::Bishop].loadFromFile("../Resources/w_bishop.png");

		res &= pieceTextures[PieceEnums::Black][PieceEnums::Pawn].loadFromFile("../Resources/b_pawn.png");
		res &= pieceTextures[PieceEnums::Black][PieceEnums::Knight].loadFromFile("../Resources/b_knight.png");
		res &= pieceTextures[PieceEnums::Black][PieceEnums::Rook].loadFromFile("../Resources/b_rook.png");
		res &= pieceTextures[PieceEnums::Black][PieceEnums::Queen].loadFromFile("../Resources/b_queen.png");
		res &= pieceTextures[PieceEnums::Black][PieceEnums::King].loadFromFile("../Resources/b_king.png");
		res &= pieceTextures[PieceEnums::Black][PieceEnums::Bishop].loadFromFile("../Resources/b_bishop.png");

		return res;
	}

	static std::shared_ptr<Object> createMainMenu()
	{
		auto mainMenu = std::make_shared<Object>("Main Menu");

		auto welcomeTextCenter = std::make_shared<Object>("Welcome Text");
		welcomeTextCenter->setPosition({ startSizeX / 2, 150 });
		mainMenu->addChild(welcomeTextCenter);

		auto welcomeText = std::make_shared<TextObject>("Welcome to Chess!", font, 50);
		sf::Vector2f textSize = welcomeText->getTextSize();
		welcomeText->setPosition(-textSize / 2.f);
		welcomeTextCenter->addChild(welcomeText);

		auto startButton = std::make_shared<Object>("Start Button");
		startButton->setPosition({ startSizeX / 2, startSizeY / 2 });
		mainMenu->addChild(startButton);

		sf::Vector2f buttonBackgroundSize{ 250, 75 };
		auto buttonBackground = makeShape<sf::RectangleShape>("Start Button Background", buttonBackgroundSize);
		buttonBackground->setPosition(-buttonBackgroundSize / 2.f);
		buttonBackground->getShape().setFillColor(buttonColor);
		startButton->addChild(buttonBackground);

		auto buttonText = std::make_shared<TextObject>("Start Text", "PLAY", font, 50);
		textSize = buttonText->getTextSize();
		buttonText->setPosition({ -textSize.x / 2, -textSize.y});
		startButton->addChild(buttonText);

		return mainMenu;
	}

	void startGUI()
	{
		if (!font.loadFromFile("../Resources/Raleway-Black.ttf"))
		{
			std::cerr << "Could not load font file.\n";
			return;
		}

		if (!loadPieceTextures())
		{
			std::cerr << "Could not load piece sprites.\n";
			return;
		}

		sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(startSizeX), static_cast<unsigned int>(startSizeY)), "Chess");
		window.setVerticalSyncEnabled(true);

		GUI::Autoscaler world{ {startSizeX, startSizeY} };

		world.addChild(createMainMenu());

		while (window.isOpen())
		{
			window.clear(sf::Color(200, 200, 150));

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
				if (event.type == sf::Event::Resized)
				{
					sf::FloatRect visibleArea(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
					window.setView(sf::View(visibleArea));
					world.onResize(window);
				}
				if (event.type == sf::Event::MouseButtonPressed)
				{
					sf::Vector2f clickPosition{ static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
					world.interact(clickPosition);
				}
			}

			window.draw(world);
			window.display();
		}
	}
}