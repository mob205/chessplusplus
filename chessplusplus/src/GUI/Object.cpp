#include "GUI/Object.h"

#include <iostream>

using GUI::Object;

void Object::addChild(std::shared_ptr<Object> object)
{
	children.push_back(object);
}

bool Object::interact(const sf::Vector2f& interactPoint)
{
	if (!getVisibility()) { return false; }
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

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!getVisibility()) { return; }

	// Combine parent and this object's transform
	sf::RenderStates transformedStates = states.transform * getTransform();

	// Draw this object
	draw_impl(target, transformedStates);

	// Draw children
	for (const auto& child : children)
	{
		target.draw(*child, transformedStates);
	}
}


bool GUI::Object::interact_impl(const sf::Vector2f& point)
{
	return false;
}

void GUI::Object::draw_impl(sf::RenderTarget& target, sf::RenderStates states) const
{
	// By default, objects do not draw anything
}

