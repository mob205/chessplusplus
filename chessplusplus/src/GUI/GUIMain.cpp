#include "GUI/GUImain.h"
#include <iostream>
#include "SFML/Graphics.hpp"
#include "Game/Settings.h"
#include "Piece/PieceEnums.h"

namespace GUI
{
    // Creates the board texture
    static bool SetupBoard(sf::RenderTexture& boardTexture)
    {
        constexpr int boardLength{ Settings::boardSize * Settings::pixelsPerTile };
        sf::Vector2u textureSize{ boardLength, boardLength };

        if (!boardTexture.create(textureSize.x, textureSize.y))
        {
            std::cerr << "Could not create board!\n";
            return false;
        }

        std::vector<sf::RectangleShape> rects(64, sf::RectangleShape{ sf::Vector2f{Settings::pixelsPerTile, Settings::pixelsPerTile} });


        sf::Vector2f center{ static_cast<float>(textureSize.x / 2), static_cast<float>(textureSize.y / 2) };

        float offsetX{ -boardLength / 2 };
        float offsetY{ offsetX };
        for (int i = 0; i < Settings::boardSize; ++i)
        {
            float curOffsetX{ offsetX };
            for (int j = 0; j < Settings::boardSize; ++j)
            {
                sf::RectangleShape& rectangle{ rects[Settings::boardSize * i + j] };
                rectangle.setFillColor((i + j) % 2 == 0 ? sf::Color::White : sf::Color::Black);
                rectangle.setPosition(sf::Vector2f{ curOffsetX, offsetY } + center);
                boardTexture.draw(rectangle);
                curOffsetX += Settings::pixelsPerTile;
            }
            offsetY += Settings::pixelsPerTile;
        }
        return true;
    }

    void startGUI()
    {
        sf::RenderWindow window(sf::VideoMode(1000, 800), "Chess");
        window.setVerticalSyncEnabled(true);


        constexpr int boardLength{ Settings::boardSize * Settings::pixelsPerTile };
        sf::RenderTexture boardTexture{};
        if (!SetupBoard(boardTexture))
        {
            std::cerr << "Could not setup board.\n";
            return;
        }

        sf::Sprite boardSprite{ boardTexture.getTexture() };

        /*sf::Texture texture;
        if (!texture.loadFromFile("2x/b_bishop.png"))
        {

        }
        sf::RectangleShape piece{ sf::Vector2f{Settings::pixelsPerTile, Settings::pixelsPerTile} };
        piece.setTexture(&texture);*/



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
                }
            }

            // Ensure board stays in center of screen, even when resizing
            sf::Vector2f screenCenter{ static_cast<float>(window.getSize().x / 2), static_cast<float>(window.getSize().y / 2) };
            boardSprite.setPosition(sf::Vector2f{ screenCenter.x - (boardLength / 2), screenCenter.y - (boardLength / 2) });
            //piece.setPosition(sprite.getPosition());


            window.draw(boardSprite);
            window.display();
        }
    }
}