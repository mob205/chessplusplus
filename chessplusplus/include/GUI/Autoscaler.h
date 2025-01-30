#pragma once

#include "SFML/Graphics.hpp"
#include "Object.h"

#include <vector>

namespace GUI
{
	// GUI object that automatically scales to match the fit aspect ratio
	class Autoscaler : public Object
	{
	public:
		Autoscaler(sf::Vector2f initialSize)
			: Object{ "" }, aspectRatio{ initialSize.x / initialSize.y }, referenceSize{ initialSize }
		{}

		void onResize(const sf::RenderWindow& window);

	private:
		float aspectRatio{};
		sf::Vector2f referenceSize{};
	};
}
