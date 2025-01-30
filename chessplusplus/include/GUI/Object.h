#pragma once

#include <vector>
#include "SFML/Graphics.hpp"

namespace GUI
{
	class Object : public sf::Drawable, public sf::Transformable
	{
	public:
		Object(const std::string& objectName)
			: name{ objectName }
		{}

		virtual ~Object() = default;

		void addChild(std::shared_ptr<Object> object);
	protected:
		std::vector<std::shared_ptr<Object>> children{};

		void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	private:
		std::string name{ "Object" };

		bool isVisible{ true };

	public:
		std::string_view getName() const { return name; }

		bool getVisibility() const { return isVisible; }
		void setVisibility(bool visibility) { isVisible = visibility; }
	};
}