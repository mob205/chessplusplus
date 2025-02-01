#pragma once

#include <vector>
#include "SFML/Graphics.hpp"

namespace GUI
{
	class Object : public sf::Drawable, public sf::Transformable
	{
	public:
		Object()
			: name{ "Object" }
		{}

		Object(const std::string& objectName)
			: name{ objectName }
		{}

		virtual ~Object() = default;

		// Adds a child object to this object. A parent will draw child objects

		/**
		*	Adds a child object to this object. Parents will draw and interact-test child objects.
		*	@param object The child object to add
		* 
		*/
		void addChild(std::shared_ptr<Object> object);

		/** 
		*	Processes an interaction (i.e. a mouse click) at a point
		*	@param point Point to test for interaction
		*	@return true if mouse click interacted with the object
		*/
		bool interact(const sf::Vector2f& point);

	protected:
		std::vector<std::shared_ptr<Object>> children{};

	private:
		std::string name{ "Object" };
		bool isVisible{ true };

		void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

		/**
		*	Implementation for checking if a point interacts with the object.
		*	The point will be transformed to this object's local space, according to its transform.
		*	@param point Point to test for interaction
		*	@return true if mouse click interacted with the object
		*/
		virtual bool interact_impl(const sf::Vector2f& point);

		/**
		*	Implementation for drawing the object to a render target.
		*	States will be transformed to this object's local space, according to its transform.	
		*/
		virtual void draw_impl(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
	public:
		std::string_view getName() const { return name; }

		/**
		*	Returns whether this object should be drawn to the screen.
		*/
		bool getVisibility() const { return isVisible; }
		void setVisibility(bool visibility) { isVisible = visibility; }
	};
}