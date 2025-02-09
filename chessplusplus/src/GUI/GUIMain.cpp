#include <iostream>
#include <memory>
#include "SFML/Graphics.hpp"

#include "Game/Settings.h"
#include "Game/Game.h"
#include "Piece/PieceEnums.h"

#include "GUI/GUIController.h"
#include "GUI/GUIMain.h"
#include "GUI/Autoscaler.h"
#include "GUI/SFMLObject.h"
#include "GUI/TextObject.h"
#include "GUI/Menus.h"

namespace GUI
{
	static PieceTextures pieceTextures(PieceEnums::MaxTeams, std::vector<sf::Texture>(PieceEnums::MaxTypes));
	static sf::Font font;

	static bool loadPieceTextures(const std::string& resourcePath)
	{
		bool res{ true };
		sf::Texture blankTexture{};
		blankTexture.create(pixelsPerTile, pixelsPerTile);
		pieceTextures[PieceEnums::White][PieceEnums::None] = blankTexture;
		pieceTextures[PieceEnums::Black][PieceEnums::None] = blankTexture;

		res &= pieceTextures[PieceEnums::White][PieceEnums::Pawn].loadFromFile(resourcePath + "w_pawn.png");
		res &= pieceTextures[PieceEnums::White][PieceEnums::Knight].loadFromFile(resourcePath + "w_knight.png");
		res &= pieceTextures[PieceEnums::White][PieceEnums::Rook].loadFromFile(resourcePath + "w_rook.png");
		res &= pieceTextures[PieceEnums::White][PieceEnums::Queen].loadFromFile(resourcePath + "w_queen.png");
		res &= pieceTextures[PieceEnums::White][PieceEnums::King].loadFromFile(resourcePath + "w_king.png");
		res &= pieceTextures[PieceEnums::White][PieceEnums::Bishop].loadFromFile(resourcePath + "w_bishop.png");

		res &= pieceTextures[PieceEnums::Black][PieceEnums::Pawn].loadFromFile(resourcePath + "b_pawn.png");
		res &= pieceTextures[PieceEnums::Black][PieceEnums::Knight].loadFromFile(resourcePath + "b_knight.png");
		res &= pieceTextures[PieceEnums::Black][PieceEnums::Rook].loadFromFile(resourcePath + "b_rook.png");
		res &= pieceTextures[PieceEnums::Black][PieceEnums::Queen].loadFromFile(resourcePath + "b_queen.png");
		res &= pieceTextures[PieceEnums::Black][PieceEnums::King].loadFromFile(resourcePath + "b_king.png");
		res &= pieceTextures[PieceEnums::Black][PieceEnums::Bishop].loadFromFile(resourcePath + "b_bishop.png");

		return res;
	}

	void startGUI()
	{
		if (!font.loadFromFile("../Resources/Raleway-Black.ttf") && !font.loadFromFile("./Resources/Raleway-Black.ttf"))
		{
			std::cerr << "Could not load font file.\n";
			system("pause");
			return;
		}

		if (!loadPieceTextures("./Resources/") && !loadPieceTextures("../Resources/"))
		{
			std::cerr << "Could not load piece sprites.\n";
			system("pause");
			return;
		}

		sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(startSizeX), static_cast<unsigned int>(startSizeY)), "Chess");
		window.setVerticalSyncEnabled(true);

		GUI::Autoscaler world{ {startSizeX, startSizeY} };

		GUIController controller{};

		auto mainMenu = createMainMenu(font, controller);
		world.addChild(mainMenu);

		auto chessMenu = createChessMenu(font, controller, pieceTextures);
		world.addChild(chessMenu);
		chessMenu->setActive(false);

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
				if (event.type == sf::Event::TextEntered)
				{
					controller.onTextEntered(event.text.unicode);
				}
			}

			controller.tick();

			window.draw(world);
			window.display();
		}
	}
}