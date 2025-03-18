// Interface to make gameobjects collide with each other
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "component.h"

namespace game {
	class ColliderObject : public Component {
	public:
		// Constructor
		ColliderObject(float radius, bool type = 0) : radius_(radius), collider_type(type) {}
		virtual ~ColliderObject() = default;

		float getRadius() const { return radius_; }
		bool getColliderType() const { return collider_type; }
		// General collision function
		bool collide(GameObject* other) const;
	protected:
		// Radius of the collider
		const float radius_;
		// Bool to determine type of collider object uses
		// false = circle, true = ray
		// Can change to enum later for more types
		const bool collider_type = false;

		bool circleCollision(GameObject* other) const;
		bool rayCollision(GameObject* other) const;
	};
}