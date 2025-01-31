#include "GUI/ShapeObject.h"

bool GUI::ShapeObject::interact_impl(const sf::Vector2f& interactPoint)
{
	return shape->getLocalBounds().contains(interactPoint);
}

void GUI::ShapeObject::draw_impl(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*shape, states);
}
