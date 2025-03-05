// Interface to make gameobjects collide with each other
#ifndef COLLIDER_OBJECT_H
#define COLLIDER_OBJECT_H

#include <glm/glm.hpp>

namespace game {
	class ColliderObject {
	public:
		// Constructor
		ColliderObject(float radius, bool type = 0) : radius_(radius), collider_type(type) {}
		virtual ~ColliderObject() = default;

		float getRadius() const { return radius_; }
		bool getColliderType() const { return collider_type; }
		virtual glm::vec3 getPosition() const = 0;
		virtual glm::vec3 getBearing() const = 0;
		bool collide(const ColliderObject* other) const;
	protected:
		// Radius of the collider
		const float radius_;
		// Bool to determine type of collider object uses
		// false = circle, true = ray
		// Can change to enum later for more types
		const bool collider_type = false;

		bool circleCollision(const ColliderObject* other) const;
		bool rayCollision(const ColliderObject* other) const;
	};
}

#endif // COLLIDER_OBJECT_H