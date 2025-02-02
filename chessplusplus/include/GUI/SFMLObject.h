#pragma once

#include "SFML/Graphics.hpp"
#include "GUI/Object.h"
#include <memory>

namespace GUI
{
	template<typename T>
	concept IsSFML = std::is_base_of_v<sf::Drawable, T>&& std::is_base_of_v<sf::Transformable, T>;

	// Object wrapper for SFML objects
	template<typename SFMLType> requires IsSFML<SFMLType>
	class SFMLObject : public Object
	{
	public:
		virtual ~SFMLObject() = default;

		SFMLObject(const std::string& name, SFMLType&& inShape)
			: Object{ name }, obj { inShape }
		{}

		SFMLObject(SFMLType&& inShape)
			: SFMLObject{ "SFML Object", std::move(inShape) }
		{}

		/**
		*	Returns a reference to the underlying object
		*/
		SFMLType& getObject() { return obj; }

	protected:
		virtual bool interact_impl(const sf::Vector2f& point) override
		{
			return obj.getLocalBounds().contains(point);
		}

		virtual void draw_impl(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override
		{
			target.draw(obj, states);
		}

	private:
		SFMLType obj;
	};

	// Helper functions for making shared pointers
	template<typename SFMLType, typename StringType, typename... Args> requires IsSFML<SFMLType> && std::convertible_to<StringType, std::string>
	std::shared_ptr<SFMLObject<SFMLType>> makeWrapper(const StringType& name, Args&&... args)
	{
		return std::make_shared<SFMLObject<SFMLType>>(name, SFMLType{ std::forward<Args>(args)... });
	}

	template<typename SFMLType> requires IsSFML<SFMLType>
	std::shared_ptr<SFMLObject<SFMLType>> makeWrapper(const std::string& name, SFMLType&& inShape)
	{
		return std::make_shared<SFMLObject<SFMLType>>(name, inShape);
	}

	// Member function specializations for shape types
	template<>
	inline bool SFMLObject<sf::CircleShape>::interact_impl(const sf::Vector2f& point)
	{
		// Origin is in the the top-left corner of the bounding box of circle, so subtract to displacement from circle center
		float r = obj.getRadius();
		sf::Vector2f diff = point - sf::Vector2f{ r, r };
		return (diff.x * diff.x + diff.y * diff.y) < (r * r);
	}
}

