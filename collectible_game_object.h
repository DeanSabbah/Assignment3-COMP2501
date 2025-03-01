#ifndef COLLECTIBLEGAMEOBJECT_H
#define COLLECTIBLEGAMEOBJECT_H

#pragma once
#include "game_object.h"

namespace game {
    class CollectibleGameObject : public GameObject {
	public:
		CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
		~CollectibleGameObject();
		void Update(double delta_time) override;
    };
}

#endif // COLLECTIBLEGAMEOBJECT_H
