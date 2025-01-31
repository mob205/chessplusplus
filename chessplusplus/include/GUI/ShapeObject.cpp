#include "ShapeObject.h"

bool GUI::ShapeObject::interact_impl(const sf::Vector2f& interactPoint)
{
	return shape->getLocalBounds().contains(interactPoint);
}

void GUI::ShapeObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!getVisibility()) { return; }

	Object::draw(target, states);
	target.draw(*shape, states.transform * getTransform());
}
