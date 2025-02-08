#pragma once

#include <memory>
#include <vector>
#include "GUI/GUIMain.h"
#include "SFML/Graphics.hpp"


class Object;
class GUIController;
class sf::Font;


namespace GUI
{
	std::shared_ptr<Object> createMainMenu(const sf::Font& font, GUIController& controller);

	std::shared_ptr<Object> createChessMenu(const sf::Font& font, GUIController& controller, const PieceTextures& textures);
}