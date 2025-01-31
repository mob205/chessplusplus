#include "GUI/TextObject.h"

#include <iostream>

namespace GUI
{
	void TextObject::draw_impl(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(text, states);
	}
}