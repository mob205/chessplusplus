#pragma once

#include "SFML/Graphics.hpp"
#include "GUI/Object.h"

#include <memory>

namespace GUI
{
	class ShapeObject : public Object
	{
	public:
		virtual ~ShapeObject() = default;

		ShapeObject(const std::string& name, std::unique_ptr<sf::Shape> inShape)
			: Object{ name }, shape { std::move(inShape) }
		{}

		ShapeObject(std::unique_ptr<sf::Shape> inShape)
			: ShapeObject{ "ShapeObject", std::move(inShape) }
		{}

		sf::Shape& getShape() { return *shape; }

	protected:
		virtual bool interact_impl(const sf::Vector2f& interactPoint) override;

		virtual void draw_impl(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;


	private:
		std::unique_ptr<sf::Shape> shape;
	};

	// Use concept for a more helpful error message if a shape isn't passed
	template<typename T>
	concept ShapeDerived = std::is_base_of_v<sf::Shape, T>;

	template<typename ShapeType, typename... Args> requires ShapeDerived<ShapeType>
	std::shared_ptr<ShapeObject> makeShape(Args&&... args)
	{
		return std::make_shared<ShapeObject>(std::make_unique<ShapeType>(std::forward<Args>(args)...));
	}

	template<typename ShapeType, typename StringType, typename... Args> requires ShapeDerived<ShapeType> && std::convertible_to<StringType, std::string>
	std::shared_ptr<ShapeObject> makeShape(const StringType& name, Args&&... args)
	{
		return std::make_shared<ShapeObject>(name, std::make_unique<ShapeType>(std::forward<Args>(args)...));
	}
}