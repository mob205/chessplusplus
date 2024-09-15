#pragma once
#include "GUI/Menu.h"
#include "SFML/Graphics.hpp"
#include <functional>

class MenuManager
{
public:
	MenuManager(Menu& startMenu);

	void setActiveMenu(int menuNumber);

	Menu& getActiveMenu();

	void addMenu(Menu& menu);
private:
	int activeMenu{};
	std::vector <std::reference_wrapper<Menu>> menus{};
};