#include "ShapeObject.h"

void GUI::ShapeObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!getVisibility()) { return; }

	Object::draw(target, states);
	target.draw(*shape, states.transform * getTransform());
}
