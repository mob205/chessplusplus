#pragma once

#include "SFML/Graphics.hpp"

namespace GUI
{
	class Object : public sf::Drawable, public sf::Transformable
	{
	public:
		virtual ~Object() = default;
	};
}