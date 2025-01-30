#pragma once

#include "SFML/Graphics.hpp"
#include <memory>
#include "GUI/Object.h"

#include <iostream>

namespace GUI
{
	class ShapeObject : public Object
	{
	public:
		virtual ~ShapeObject() = default;

		ShapeObject(std::unique_ptr<sf::Shape> inShape)
			: Object{ "ShapeObject" }, shape{ std::move(inShape) }
		{}

		ShapeObject(const std::string& name, std::unique_ptr<sf::Shape> inShape)
			: Object{ name }, shape { std::move(inShape) }
		{}

		sf::Shape& getShape() { return *shape; }

	private:
		std::unique_ptr<sf::Shape> shape;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};

	// Use concept for a more helpful error message if a shape isn't passed
	template<typename T>
	concept ShapeDerived = std::is_base_of_v<sf::Shape, T>;

	template<typename ShapeType, typename... Args> requires ShapeDerived<ShapeType>
	std::shared_ptr<ShapeObject> makeShape(Args&&... args)
	{
		return std::make_shared<ShapeObject>(std::make_unique<ShapeType>(std::forward<Args>(args)...));
	}

	template<typename ShapeType, typename U, typename... Args> requires ShapeDerived<ShapeType> && std::convertible_to<U, std::string>
	std::shared_ptr<ShapeObject> makeShape(const U& name, Args&&... args)
	{
		return std::make_shared<ShapeObject>(name, std::make_unique<ShapeType>(std::forward<Args>(args)...));
	}
}