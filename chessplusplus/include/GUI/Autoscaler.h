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
		Autoscaler(const std::string& name, const sf::Vector2f& initialSize)
			: Object{ name }, aspectRatio{ initialSize.x / initialSize.y }, referenceSize{ initialSize }
		{}

		Autoscaler(const sf::Vector2f& initialSize)
			: Autoscaler{ "", initialSize }
		{}

		void onResize(const sf::RenderWindow& window);
		
	protected:
		virtual bool containsPoint(const sf::Vector2f& interactPoint) override;

	private:
		float aspectRatio{};
		sf::Vector2f referenceSize{};
	};
}
