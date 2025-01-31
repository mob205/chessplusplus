#include "Object.h"

#include <iostream>

using GUI::Object;

void Object::addChild(std::shared_ptr<Object> object)
{
	children.push_back(object);
}

bool Object::interact(const sf::Vector2f& interactPoint)
{
	bool res{};

	const sf::Vector2f transformedPoint = getInverseTransform().transformPoint(interactPoint);

	// Interact with this object
	res |= interact_impl(transformedPoint);

	if (res && !getName().empty())
	{
		std::cout << "Interacted with " << getName() << ".\n";
	}

	// Interact with children
	for (const auto& child : children)
	{
		res |= child->interact(transformedPoint);
	}
	return res;
}

bool GUI::Object::interact_impl(const sf::Vector2f& point)
{
	return false;
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!getVisibility()) { return; }
	for (const auto& child : children)
	{
		target.draw(*child, states.transform * getTransform());
	}
}
