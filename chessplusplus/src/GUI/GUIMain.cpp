#include <iostream>
#include "SFML/Graphics.hpp"
#include "Game/Settings.h"
#include "Game/Game.h"

#include "GUI/TestButton.h"
#include "GUI/SwitchMenuButton.h"

#include "GUI/GUImain.h"
#include "GUI/Menu.h"
#include "GUI/MenuManager.h"

static const sf::Color buttonColor{ 215, 215, 215 };

namespace GUI
{
    // Creates the board texture
    static bool SetupBoard(sf::RenderTexture& boardTexture)
    {
        sf::Vector2u textureSize{ boardLength, boardLength };

        if (!boardTexture.create(textureSize.x, textureSize.y))
        {
            std::cerr << "Could not create board!\n";
            return false;
        }

        std::vector<sf::RectangleShape> rects(Settings::boardSize * Settings::boardSize, sf::RectangleShape{ sf::Vector2f{pixelsPerTile, pixelsPerTile} });

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
                curOffsetX += pixelsPerTile;
            }
            offsetY += pixelsPerTile;
        }
        boardTexture.display();
        return true;
    }

    static void SetupMainMenu(Menu& menu, const sf::Font& font, MenuManager& manager)
    {

        sf::Text welcome{ "Welcome to Chess!", font, 90 };
        welcome.setPosition({ (menuSize - welcome.getGlobalBounds().width) / 2, 100});
        welcome.setFillColor(sf::Color::Black);
        menu.addElement(welcome);


        auto startButton{ std::make_unique<SwitchMenuButton>(sf::Vector2f{buttonWidth, buttonHeight}, buttonColor, "Start Game", font, 45, manager, 1) };
        startButton->setPosition({ (menuSize - buttonWidth) / 2, 300 });
        menu.addButton(std::move(startButton));

        auto loadButton{ std::make_unique<TestButton>(sf::Vector2f{buttonWidth, buttonHeight }, buttonColor, "Load Game", font, 45)};
        loadButton->setPosition({ (menuSize - buttonWidth) / 2, 500 });
        menu.addButton(std::move(loadButton));
    }

    static void SetupBoardMenu(Menu& menu, const sf::Font& font, MenuManager& manager)
    {
        sf::RenderTexture boardTexture{};
        SetupBoard(boardTexture);
        sf::Sprite boardSprite{ boardTexture.getTexture() };
        boardSprite.setPosition(static_cast<float>((menuSize - boardTexture.getSize().x) / 2), static_cast<float>((menuSize - boardTexture.getSize().y) / 2) + 50);
        menu.addElement(boardSprite);

        auto quitButton{ std::make_unique<SwitchMenuButton>(sf::Vector2f{buttonWidth, buttonHeight}, buttonColor, "Quit", font, 45, manager, 0) };
        quitButton->setPosition({ 50, 120});
        menu.addButton(std::move(quitButton));
    }

    void startGUI()
    {
        sf::RenderWindow window(sf::VideoMode(1000, 800), "Chess");
        window.setVerticalSyncEnabled(true);
        
        sf::Font font;
        if (!font.loadFromFile("../Resources/Raleway-Black.ttf"))
        {
            std::cerr << "Could not load font file.\n";
            return;
        }


        Menu mainMenu{ {menuSize, menuSize } };
        Menu boardMenu{ {menuSize, menuSize} };
        MenuManager menuManager{ mainMenu };

        SetupMainMenu(mainMenu, font, menuManager);
        SetupBoardMenu(boardMenu, font, menuManager);

        menuManager.addMenu(boardMenu);


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
                    menuManager.getActiveMenu().onButtonPress(sf::Vector2f{static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)});
                }
            }

            // Ensure board stays in center of screen, even when resizing
            sf::Vector2f screenCenter{ static_cast<float>(window.getSize().x / 2), static_cast<float>(window.getSize().y / 2) };
            menuManager.getActiveMenu().setPosition(sf::Vector2f{screenCenter.x - (menuSize / 2), screenCenter.y - (menuSize / 2)});

            window.draw(menuManager.getActiveMenu());
            window.display();
        }
    }
}