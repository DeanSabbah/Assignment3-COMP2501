#include "collider_object.h"
#include "game_object.h"

namespace game {
	bool ColliderObject::circleCollision(GameObject* other) const {
		// Get the distance between the two objects
		float distance = glm::distance(parent->GetPosition(), other->GetPosition());
		// If the distance is less than the sum of the radii, then the objects are colliding
		if (distance < radius_ + other->GetComponent<ColliderObject>()->getRadius()) {
			return true;
		}
		return false;
	}

	bool ColliderObject::collide(GameObject* other) const {
		return collider_type ? rayCollision(other) : circleCollision(other);
	}

	bool ColliderObject::rayCollision(GameObject* other) const {
		// Ray origin and direction
		glm::vec3 rayOrigin = parent->GetPosition();
		glm::vec3 rayDirection = glm::normalize(parent->GetBearing());

		// Circle center and radius
		glm::vec3 circleCenter = other->GetPosition();
		float circleRadius = other->GetComponent<ColliderObject>()->getRadius();

		// Vector from ray origin to circle center
		glm::vec3 oc = circleCenter - rayOrigin;

		// Project oc onto the ray direction
		float t = glm::dot(2.0f * oc, rayDirection);

		// Closest point on the ray to the circle center
		glm::vec3 closestPoint = rayOrigin + t * rayDirection;

		// Distance from the closest point to the circle center
		float distance = glm::length(closestPoint - circleCenter);

		// Check if the distance is less than the circle's radius
		bool intersect = distance <= circleRadius;

		// Check if the closest point is within the max distance from the ray origin
		float distanceFromOrigin = glm::length(closestPoint - rayOrigin);
		return intersect && distanceFromOrigin <= (radius_ + circleRadius);
	}
}