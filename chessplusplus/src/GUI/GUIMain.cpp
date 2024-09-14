#include "GUI/GUImain.h"
#include <iostream>
#include "SFML/Graphics.hpp"
#include "Game/Settings.h"
#include "Game/Game.h"
#include "GUI/TestButton.h"
#include "GUI/Menu.h"

namespace GUI
{
    // Creates the board texture
    static bool SetupBoard(sf::RenderTexture& boardTexture)
    {
        sf::Vector2u textureSize{ Settings::boardLength, Settings::boardLength };

        if (!boardTexture.create(textureSize.x, textureSize.y))
        {
            std::cerr << "Could not create board!\n";
            return false;
        }

        std::vector<sf::RectangleShape> rects(Settings::boardSize * Settings::boardSize, sf::RectangleShape{ sf::Vector2f{Settings::pixelsPerTile, Settings::pixelsPerTile} });


        sf::Vector2f center{ static_cast<float>(textureSize.x / 2), static_cast<float>(textureSize.y / 2) };

        float offsetX{ -Settings::boardLength / 2 };
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
        boardTexture.display();
        return true;
    }

    static void SetupMenu(Menu& menu, const sf::Font& font)
    {
        constexpr int size{ Settings::menuSize };


        constexpr int buttonWidth{ 300 };
        constexpr int buttonHeight{ 100 };
        const sf::Color buttonColor{ 215, 215, 215 };


        sf::Text welcome{ "Welcome to Chess!", font, 90 };
        welcome.setPosition({ (size - welcome.getGlobalBounds().width) / 2, 100});
        welcome.setFillColor(sf::Color::Black);
        menu.addElement(welcome);

        auto startButton{ std::make_unique<TestButton>(sf::Vector2f{buttonWidth, buttonHeight}, buttonColor, "Start Game", font, 45) };
        startButton->setPosition({ (size - buttonWidth) / 2, 300 });
        menu.addButton(std::move(startButton));

        auto loadButton{ std::make_unique<TestButton>(sf::Vector2f{buttonWidth, buttonHeight }, buttonColor, "Load Game", font, 45)
    };
        loadButton->setPosition({ (size - buttonWidth) / 2, 500 });
        menu.addButton(std::move(loadButton));
    }

    void startGUI()
    {
        sf::RenderWindow window(sf::VideoMode(1000, 800), "Chess");
        window.setVerticalSyncEnabled(true);


        sf::RenderTexture boardTexture{};
        if (!SetupBoard(boardTexture))
        {
            std::cerr << "Could not setup board.\n";
            return;
        }

        sf::Sprite boardSprite{ boardTexture.getTexture() };

        sf::Font font;
        if (!font.loadFromFile("../Resources/Raleway-Black.ttf"))
        {
            std::cerr << "Could not load font file.\n";
            return;
        }


        Menu menu{ {Settings::menuSize, Settings::menuSize } };
        SetupMenu(menu, font);
      
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
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    menu.onButtonPress(sf::Vector2f{ static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) });
                }
            }

            // Ensure board stays in center of screen, even when resizing

            sf::Vector2f screenCenter{ static_cast<float>(window.getSize().x / 2), static_cast<float>(window.getSize().y / 2) };
            boardSprite.setPosition(sf::Vector2f{ screenCenter.x - (Settings::boardLength / 2), screenCenter.y - (Settings::boardLength / 2) });
            menu.setPosition(sf::Vector2f{ screenCenter.x - (Settings::menuSize / 2), screenCenter.y - (Settings::menuSize / 2)});

            //window.draw(boardSprite);
            window.draw(menu);
            window.display();
        }
    }
}