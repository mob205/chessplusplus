#include "MenuManager.h"
#include "SFML/Graphics.hpp"
#include <iostream>

MenuManager::MenuManager(Menu& menu)
{
	addMenu(menu);
}

void MenuManager::setActiveMenu(int num)
{
	if (num < 0 || num >= menus.size())
	{
		std::cerr << "setActiveMenu: Index out of range!\n";
	}
	else
	{
		std::cout << "Switching to menu " << num << '\n';
		activeMenu = num;
	}
}

void MenuManager::addMenu(Menu& menu)
{
	menus.push_back(menu);
}

Menu& MenuManager::getActiveMenu()
{
	return menus[activeMenu];
}

