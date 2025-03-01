#include "collectible_game_object.h"

namespace game {
	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		health = 1;
	}

	CollectibleGameObject::~CollectibleGameObject() {
		GameObject::~GameObject();
	}

	void CollectibleGameObject::Update(double delta_time) {
		GameObject::Update(delta_time);
	}

}
