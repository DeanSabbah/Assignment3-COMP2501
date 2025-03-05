#ifndef COLLECTIBLEGAMEOBJECT_H
#define COLLECTIBLEGAMEOBJECT_H

#pragma once
#include "game_object.h"
#include "collider_object.h"

namespace game {
    class CollectibleGameObject : public GameObject, public ColliderObject {
	public:
		CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec2& scale, const float radius);
		~CollectibleGameObject();
		void Update(double delta_time) override;
		// Get position for Collider
		glm::vec3 ColliderObject::getPosition() const { return position_; }
		// Get bearing for Collider
		glm::vec3 ColliderObject::getBearing() const { return GetBearing(); }
    };
}

#endif // COLLECTIBLEGAMEOBJECT_H
