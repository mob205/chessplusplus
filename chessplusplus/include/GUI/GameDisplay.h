#pragma once

#include "SFML/Graphics.hpp"
#include "Object.h"

#include <vector>

namespace GUI
{
	class GameDisplay : public sf::Drawable, public sf::Transformable
	{
	public:
		GameDisplay(sf::Vector2f initialSize)
			: aspectRatio{ initialSize.x / initialSize.y }, referenceSize{ initialSize }
		{}

		void addObject(std::shared_ptr<Object> obj)
		{
			objects.push_back(obj);
		}

		//void addObject(std::shared_ptr<Object> obj);
		void onResize(const sf::RenderWindow& window);

	private:
		float aspectRatio{};
		sf::Vector2f referenceSize{};

		void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

		std::vector<std::shared_ptr<Object>> objects;
	};
}
