#include "Object.h"

using GUI::Object;

void Object::addChild(std::shared_ptr<Object> object)
{
	children.push_back(object);
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!getVisibility()) { return; }
	for (const auto& child : children)
	{
		target.draw(*child, states.transform * getTransform());
	}
}
