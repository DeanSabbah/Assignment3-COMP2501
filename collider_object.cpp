#include "collider_object.h"

namespace game {
	bool ColliderObject::circleCollision(const ColliderObject* other) const {
		// Get the distance between the two objects
		float distance = glm::distance(getPosition(), other->getPosition());
		// If the distance is less than the sum of the radii, then the objects are colliding
		if (distance < radius_ + other->getRadius()) {
			return true;
		}
		return false;
	}

	bool ColliderObject::rayCollision(const ColliderObject* other) const {
		// TODO: Implement ray collision
		return false;
	}
}