#include "collider_object.h"

namespace game {
	// General collision function
	bool ColliderObject::collide(const ColliderObject* other) const {
		if (!collider_type) {
			return circleCollision(other);
		}
		else {
			return rayCollision(other);
		}
	}
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
        // Ray origin and direction
        glm::vec3 rayOrigin = getPosition();
        glm::vec3 rayDirection = glm::normalize(getBearing());

        // Circle center and radius
        glm::vec3 circleCenter = other->getPosition();
        float circleRadius = other->getRadius();

        // Vector from ray origin to circle center
        glm::vec3 oc = circleCenter - rayOrigin;

        // Project oc onto the ray direction
        float t = glm::dot(oc, rayDirection);

        // Closest point on the ray to the circle center
        glm::vec3 closestPoint = rayOrigin + t * rayDirection;

        // Distance from the closest point to the circle center
        float distance= glm::length(closestPoint - circleCenter);

        // Check if the distance is less than the circle's radius
        bool intersect = distance <= circleRadius;

        // Check if the closest point is within the max distance from the ray origin
        float distanceFromOrigin = glm::length(closestPoint - rayOrigin);
        return intersect && distanceFromOrigin <= (radius_ + circleRadius);
    }
}