#include "collectible_game_object.h"

namespace game {
	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec2 scale, const float radius)
		: GameObject(position, geom, shader, texture, scale), ColliderObject(radius) {
		health = 1;
	}

	CollectibleGameObject::~CollectibleGameObject() {
		GameObject::~GameObject();
	}

	void CollectibleGameObject::Update(double delta_time) {
		GameObject::Update(delta_time);
	}

}
