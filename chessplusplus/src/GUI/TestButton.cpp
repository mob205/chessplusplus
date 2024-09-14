#include "GUI/TestButton.h"
#include <iostream>

void TestButton::onClick()
{
	std::cout << "I am a button and I've been clicked!\n";
}

void TestButton::onRelease()
{
	std::cout << "I am a button and I've been released!\n";
}

void TestButton::onUnclick()
{
	std::cout << "I am a button and I've been unclicked!\n";
}