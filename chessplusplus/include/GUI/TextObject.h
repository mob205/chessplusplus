#pragma once

#include "GUISettings.h"
#include "SFML/Graphics.hpp"
#include "GUI/Object.h"

#include <memory>

namespace GUI
{
	class TextObject : public Object
	{
	public:
		virtual ~TextObject() {};

		TextObject(const std::string& name, const std::string& textString, const sf::Font& font, unsigned int fontSize)
			: Object{ name }, text{ textString, font, fontSize }
		{
			text.setFillColor(sf::Color::Black);
		}
		
		TextObject(const std::string& textString, const sf::Font& font, unsigned int fontSize)
			: TextObject{ "Text Object", textString, font, fontSize }
		{}

		const std::string& getText() const
		{
			return text.getString();
		}

		sf::Vector2f getTextSize() const
		{
			return text.getLocalBounds().getSize();
		}

		void setText(const std::string& newText)
		{
			text.setString(newText);
		}

	private:
		sf::Text text;

		void draw_impl(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}