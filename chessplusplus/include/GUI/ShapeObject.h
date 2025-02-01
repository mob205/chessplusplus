#pragma once

#include "SFML/Graphics.hpp"
#include "GUI/Object.h"

#include <memory>

namespace GUI
{
	template<typename T>
	concept ShapeDerived = std::is_base_of_v<sf::Shape, T>;

	template<typename ShapeType> requires ShapeDerived<ShapeType>
	class ShapeObject : public Object
	{
	public:
		virtual ~ShapeObject() = default;

		ShapeObject(const std::string& name, ShapeType&& inShape)
			: Object{ name }, shape { inShape }
		{}

		ShapeObject(ShapeType&& inShape)
			: ShapeObject{ "ShapeObject", inShape }
		{}

		ShapeType& getShape() { return shape; }

	protected:
		virtual bool interact_impl(const sf::Vector2f& interactPoint) override
		{
			return shape.getLocalBounds().contains(interactPoint);
		}

		virtual void draw_impl(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
		{
			target.draw(shape, states);
		}

	private:
		ShapeType shape;
	};

	template<typename ShapeType, typename... Args> requires ShapeDerived<ShapeType>
	std::shared_ptr<ShapeObject<ShapeType>> makeShape(Args&&... args)
	{
		return std::make_shared<ShapeObject<ShapeType>>(ShapeType{std::forward<Args>(args)...});
	}

	template<typename ShapeType, typename StringType, typename... Args> requires ShapeDerived<ShapeType> && std::convertible_to<StringType, std::string>
	std::shared_ptr<ShapeObject<ShapeType>> makeShape(const StringType& name, Args&&... args)
	{
		return std::make_shared<ShapeObject<ShapeType>>(name, ShapeType{ std::forward<Args>(args)... });
	}
}

