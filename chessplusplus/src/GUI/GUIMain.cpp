#include <iostream>
#include <memory>
#include "SFML/Graphics.hpp"
#include "Game/Settings.h"
#include "Game/Game.h"
#include "Piece/PieceEnums.h"

#include "GUI/Autoscaler.h"
#include "GUI/ShapeObject.h"


namespace GUI
{
	const sf::Color buttonColor{ 215, 215, 215 };

	static std::vector<std::vector<sf::Texture>> pieceTextures(PieceEnums::MaxTeams, std::vector<sf::Texture>(PieceEnums::MaxTypes));

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

	void startGUI()
	{
		sf::Font font;
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

		sf::Vector2f startingSize{ 1080.0f, 900.0f };
		sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(startingSize.x), static_cast<unsigned int>(startingSize.y)), "Chess");
		window.setVerticalSyncEnabled(true);

		GUI::Autoscaler world{ startingSize };

		auto ul = makeShape<sf::RectangleShape>("Top Left", sf::Vector2f{10, 10});
		ul->getShape().setFillColor(sf::Color::Red);
		ul->setPosition(sf::Vector2f{ 0,0 });

		auto ur = makeShape<sf::RectangleShape>("Top Right", sf::Vector2f{10, 10});
		ur->getShape().setFillColor(sf::Color::Green);
		ur->setPosition(sf::Vector2f{ startingSize.x - 10, 0 });

		auto ll = makeShape<sf::RectangleShape>("Bottom Left", sf::Vector2f{ 10, 10 });
		ll->getShape().setFillColor(sf::Color::Yellow);
		ll->setPosition(sf::Vector2f{ 0, startingSize.y - 10 });

		auto lr = makeShape<sf::RectangleShape>("Bottom Right", sf::Vector2f{10, 10});
		lr->getShape().setFillColor(sf::Color::Blue);
		lr->setPosition(sf::Vector2f{ startingSize.x - 10, startingSize.y - 10 });

		world.addChild(ul);
		world.addChild(ur);
		world.addChild(ll);
		world.addChild(lr);

		ul->setVisibility(false);

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