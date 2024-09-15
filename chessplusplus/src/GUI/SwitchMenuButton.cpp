#include "GUI/SwitchMenuButton.h"

void SwitchMenuButton::onClick()
{
	menuManager.setActiveMenu(menuIndex);
}

void SwitchMenuButton::onRelease()
{

}

void SwitchMenuButton::onUnclick()
{

}