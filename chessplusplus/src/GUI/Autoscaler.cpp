#include "GUI/Autoscaler.h"
#include "GUI/Object.h"

#include <iostream>

using GUI::Autoscaler;

void Autoscaler::onResize(const sf::RenderWindow& window)
{
	sf::Vector2f windowSize = { static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) };
	sf::Vector2f displaySize{};

	// Resize display to fit inside new window size at target aspect ratio
	if (windowSize.x < aspectRatio * windowSize.y)
	{
		// x is the limiting dimension, so calculate size based off x
		displaySize = { windowSize.x, windowSize.x / aspectRatio };
	}
	else
	{
		// y is the limiting dimension
		displaySize = { aspectRatio * windowSize.y, windowSize.y };
	}

	// Set scale to affect child objects
	float scaleFactor = displaySize.x / referenceSize.x;
	setScale({ scaleFactor, scaleFactor });

	// Center the display in the center of the new window
	sf::Vector2f margin = windowSize - displaySize;
	sf::Vector2f offset = sf::Vector2f{ margin.x / 2, margin.y / 2 };
	setPosition(offset);
}

bool GUI::Autoscaler::containsPoint(const sf::Vector2f& interactPoint)
{
	return interactPoint.x > 0 && interactPoint.x <= referenceSize.x
		&& interactPoint.y > 0 && interactPoint.y <= referenceSize.y;
}



