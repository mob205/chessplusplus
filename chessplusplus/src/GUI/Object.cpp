#include "GUI/Object.h"

#include <iostream>

using GUI::Object;

void Object::addChild(std::shared_ptr<Object> object)
{
	// TODO: Add more in-depth safeguards to prevent recursive hierarchies
	if (object.get() == this)
	{
		std::cerr << "Attempted to add self as a child!";
		return;
	}


	if (!object)
	{
		std::cerr << "Attempted to add a null child!";
		return;
	}

	children.push_back(object);
}

bool Object::interact(const sf::Vector2f& interactPoint)
{
	if (!getInteractable()) { return false; }

	const sf::Vector2f transformedPoint = getInverseTransform().transformPoint(interactPoint);

	// Check for interaction with self
	if (interact_impl(transformedPoint) && !getName().empty())
	{
		// Assumes that an interactable parent does not have interactable children
		std::cout << "Interacted with " << getName() << ".\n";

		if (onInteractEvent) { onInteractEvent(); }
		return true;
	}

	// Check for interaction with children
	for (const auto& child : children)
	{
		// Assumes that only one child can be interacted with at a time
		if (child->interact(transformedPoint))
		{
			if (onInteractEvent) { onInteractEvent(); }
			return true;
		}
	}
	return false;
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!getVisibility()) { return; }

	// Combine parent and this object's transform
	sf::RenderStates transformedStates = states.transform * getTransform();

	// Draw this object
	draw_impl(target, transformedStates);

	if (!getChildrenVisibility()) { return; }

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

