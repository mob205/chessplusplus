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
			: ShapeObject{ "ShapeObject", std::move(inShape) }
		{}

		ShapeType& getShape() { return shape; }

	protected:
		virtual bool interact_impl(const sf::Vector2f& point) override
		{
			return shape.getLocalBounds().contains(point);
		}

		virtual void draw_impl(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
		{
			target.draw(shape, states);
		}

	private:
		ShapeType shape;
	};

	// Helper functions for making shared pointers
	template<typename ShapeType, typename StringType, typename... Args> requires ShapeDerived<ShapeType> && std::convertible_to<StringType, std::string>
	std::shared_ptr<ShapeObject<ShapeType>> makeShape(const StringType& name, Args&&... args)
	{
		return std::make_shared<ShapeObject<ShapeType>>(name, ShapeType{ std::forward<Args>(args)... });
	}

	template<typename ShapeType> requires ShapeDerived<ShapeType> 
	std::shared_ptr<ShapeObject<ShapeType>> makeShape(const std::string& name, ShapeType&& inShape)
	{
		return std::make_shared<ShapeObject<ShapeType>>(name, inShape);
	}

	// Member function specializations for shape types
	template<>
	inline bool ShapeObject<sf::CircleShape>::interact_impl(const sf::Vector2f& point)
	{
		// Origin is in the the top-left corner of the bounding box of circle, so subtract to displacement from circle center
		float r = shape.getRadius();
		sf::Vector2f diff = point - sf::Vector2f{ r, r };
		return (diff.x * diff.x + diff.y * diff.y) < (r * r);
	}
}

