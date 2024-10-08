#include <iostream>
#include "SFML/Graphics.hpp"
#include "Game/Settings.h"
#include "Game/Game.h"
#include "Piece/PieceEnums.h"

#include "GUI/TestButton.h"
#include "GUI/SwitchMenuButton.h"
#include "GUI/PromoButton.h"
#include "GUI/TileButton.h"
#include "GUI/UndoButton.h"
#include "GUI/SaveButton.h"
#include "GUI/LoadButton.h"

#include "GUI/GUImain.h"
#include "GUI/Menu.h"
#include "GUI/GameMenu.h"
#include "GUI/MenuManager.h"


namespace GUI
{
    const sf::Color buttonColor{ 215, 215, 215 };

    static std::vector<std::vector<sf::Texture>> pieceTextures(PieceEnums::MaxTeams, std::vector<sf::Texture>(PieceEnums::MaxTypes));

    static bool loadPieceTextures()
    {
        bool res{true};
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

    static void setupBoard(GameMenu& menu)
    {

        sf::Vector2f center{ static_cast<float>(menuSize / 2), static_cast<float>(menuSize / 2) - 100};

        float offsetX{ -boardLength / 2 };
        float offsetY{ boardLength / 2 };
        for (int i = 0; i < Settings::boardSize; ++i)
        {
            float curOffsetX{ offsetX };
            for (int j = 0; j < Settings::boardSize; ++j)
            {
                auto tileButton{ std::make_unique<TileButton>(sf::Vector2f{pixelsPerTile, pixelsPerTile}, (i + j) % 2 == 0 ? sf::Color::White : sf::Color{100, 100, 100})};
                tileButton->setOffset(sf::Vector2f{ curOffsetX - 150, offsetY });
                tileButton->recenter(center);
                menu.addTileButton(std::move(tileButton));

                // Fill columns left to right
                curOffsetX += pixelsPerTile;
            }
            // Fill rows bottom to top
            offsetY -= pixelsPerTile;
        }
    }

    static void setupMainMenu(Menu& menu, const sf::Font& font, MenuManager& manager)
    {
        sf::Text welcome{ "Welcome to Chess!", font, 90 };
        welcome.setPosition({ (menuSize - welcome.getGlobalBounds().width) / 2, 100});
        welcome.setFillColor(sf::Color::Black);
        menu.addElement(welcome);

        auto startButton{ std::make_unique<SwitchMenuButton>(sf::Vector2f{buttonWidth, buttonHeight}, buttonColor, "Play", font, 45, manager, 1) };
        startButton->setPosition({ (menuSize - buttonWidth) / 2, 300 });
        menu.addButton(std::move(startButton));

        /*auto loadButton{ std::make_unique<TestButton>(sf::Vector2f{buttonWidth, buttonHeight }, buttonColor, "Load Game", font, 45)};
        loadButton->setPosition({ (menuSize - buttonWidth) / 2, 500 });
        menu.addButton(std::move(loadButton));*/
    }

    static void setupBoardMenu(GameMenu& menu, const sf::Font& font, MenuManager& manager)
    {
        setupBoard(menu);

        auto quitButton{ std::make_unique<SwitchMenuButton>(sf::Vector2f{buttonWidth, buttonHeight}, buttonColor, "Quit", font, 45, manager, 0) };
        quitButton->setPosition({ 40, 120});
        menu.addButton(std::move(quitButton));

        auto undoButton{ std::make_unique<UndoButton>(sf::Vector2f{buttonWidth, buttonHeight}, buttonColor, "Undo", font, 45, menu) };
        undoButton->setPosition({ 330, 120 });
        menu.addButton(std::move(undoButton));

        auto loadButton{ std::make_unique<LoadButton>(sf::Vector2f{buttonWidth, buttonHeight}, buttonColor, "Load", font, 45, menu) };
        loadButton->setPosition({ 620, 120 });
        menu.addButton(std::move(loadButton));

        auto saveButton{ std::make_unique<SaveButton>(sf::Vector2f{buttonWidth, buttonHeight}, buttonColor, "Save", font, 45, menu) };
        saveButton->setPosition({ 910, 120 });
        menu.addButton(std::move(saveButton));

        auto log{ std::make_unique<sf::Text>("Stuff goes here", font, 30) };
        log->setFillColor(sf::Color::Black);
        log->setPosition({ 775, 275 });
        menu.setLogText(std::move(log));

        auto saveFileText{ std::make_unique<sf::Text>("", font, 25) };
        saveFileText->setFillColor(sf::Color::Black);
        saveFileText->setPosition(675, 125);
        menu.setSaveText(std::move(saveFileText));

        sf::RectangleShape saveTextBackground{ sf::Vector2f{475, 50} };
        saveTextBackground.setPosition(650, 215);
        saveTextBackground.setFillColor({ 100, 100, 100 });
        menu.addElement(saveTextBackground);

        sf::Text saveLabel{ "Save Name (type to enter): ", font, 25 };
        saveLabel.setFillColor(sf::Color::Black);
        saveLabel.setPosition(300, 225);
        menu.addElement(saveLabel);

        auto turnCounter{ std::make_unique<sf::Text>("Turn XX - White to Move", font, 30) };
        turnCounter->setFillColor(sf::Color::Black);
        turnCounter->setPosition({ 275, 225 });
        menu.setTurnCounter(std::move(turnCounter));

        sf::RectangleShape promotionBackground{ sf::Vector2f{450, 100} };
        promotionBackground.setFillColor({ 150, 150, 100 });

        sf::Text promotionText{ "Select a piece to promote to.", font, 30};
        promotionText.setFillColor(sf::Color::Black);

        auto queenButton{ std::make_unique<PromoButton>(sf::Vector2f{50, 50}, buttonColor, pieceTextures[PieceEnums::White][PieceEnums::Queen], menu, 'Q') };
        auto knightButton{ std::make_unique<PromoButton>(sf::Vector2f{50, 50}, buttonColor, pieceTextures[PieceEnums::White][PieceEnums::Knight], menu, 'N') };
        auto bishopButton{ std::make_unique<PromoButton>(sf::Vector2f{50, 50}, buttonColor, pieceTextures[PieceEnums::White][PieceEnums::Bishop], menu, 'B') };
        auto rookButton{ std::make_unique<PromoButton>(sf::Vector2f{50, 50}, buttonColor, pieceTextures[PieceEnums::White][PieceEnums::Rook], menu, 'R') };

        queenButton->setOffset({ 25, 40 });
        knightButton->setOffset({ 142, 40 });
        bishopButton->setOffset({ 259, 40 });
        rookButton->setOffset({ 375, 40 });

        sf::Vector2f center{};
        queenButton->recenter(center);
        knightButton->recenter(center);
        bishopButton->recenter(center);
        rookButton->recenter(center);

        sf::RenderTexture& promoTex{ menu.getPromoTexture() };
        promoTex.draw(promotionBackground);
        promoTex.draw(promotionText);

        promoTex.draw(*queenButton);
        promoTex.draw(*knightButton);
        promoTex.draw(*bishopButton);
        promoTex.draw(*rookButton);

        menu.addPromoButton(std::move(queenButton));
        menu.addPromoButton(std::move(knightButton));
        menu.addPromoButton(std::move(bishopButton));
        menu.addPromoButton(std::move(rookButton));
    }

    void startGUI()
    {
        sf::RenderWindow window(sf::VideoMode(1200, 1000), "Chess");
        window.setVerticalSyncEnabled(true);
        
        sf::Font font;
        if (!font.loadFromFile("../Resources/Raleway-Black.ttf"))
        {
            std::cerr << "Could not load font file.\n";
            return;
        }

        if (!loadPieceTextures())
        {
            std::cerr << "Could not load piece sprites.\n";
        }

        Menu mainMenu{ {menuSize, menuSize } };
        GameMenu boardMenu{ {menuSize, menuSize}, pieceTextures };
        MenuManager menuManager{ mainMenu };

        setupMainMenu(mainMenu, font, menuManager);
        setupBoardMenu(boardMenu, font, menuManager);

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
                    sf::Vector2f screenCenter{ static_cast<float>(window.getSize().x / 2), static_cast<float>(window.getSize().y / 2) };
                    menuManager.getActiveMenu().onResize(screenCenter);
                }
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    menuManager.getActiveMenu().onButtonPress(sf::Vector2f{static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)});
                }
                if (event.type == sf::Event::MouseButtonReleased)
                {
                    menuManager.getActiveMenu().onButtonRelease(sf::Vector2f{ static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) });
                }
                if (event.type == sf::Event::TextEntered)
                {
                    menuManager.getActiveMenu().onType(event.text.unicode);
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