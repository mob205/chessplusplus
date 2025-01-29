#pragma once

#include "SFML/Graphics.hpp"
#include <memory>
#include "GUI/Object.h"

namespace GUI
{
	class ShapeObject : public Object
	{
	public:
		virtual ~ShapeObject() = default;

		ShapeObject(std::unique_ptr<sf::Shape> inShape)
			: shape{ std::move(inShape) }
		{}

		template<typename ShapeType, typename... Args>
		ShapeObject(Args&&... args)
			: shape{ std::make_unique<ShapeType>(std::forward<Args>(args)...) }
		{}

		sf::Shape& getShape() { return *shape; }

	private:
		std::unique_ptr<sf::Shape> shape;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			sf::Transform transform;
			//target.draw(*shape, getTransform() * states.transform);
			target.draw(*shape, states.transform * getTransform());
		}
	};

	// Use concept for a more helpful error message if a shape isn't passed
	template<typename T>
	concept ShapeDerived = std::is_base_of_v<sf::Shape, T>;

	template<typename ShapeType, typename... Args> requires ShapeDerived<ShapeType>
	std::shared_ptr<ShapeObject> makeShape(Args&&... args)
	{
		return std::make_shared<ShapeObject>(std::make_unique<ShapeType>(args...));
	}
}