#include "projectile_game_object.h"
#include <iostream>

namespace game {
	ProjectileGameObject::ProjectileGameObject(const glm::vec3& position, glm::vec3& bearing, Geometry* geom, Shader* shader, GLuint texture, glm::vec2& scale, const float speed, const int damage, const float time, const float radius, const bool type) : GameObject(position, geom, shader, texture, scale), ColliderObject(radius, type), speed_(speed), damage_(damage) {
		timer = new Timer();
		timer->Start(time);
		SetRotation(glm::atan(bearing.y, bearing.x));
		setTexture(texture);
	}
	ProjectileGameObject::~ProjectileGameObject() {
		delete timer;
		GameObject::~GameObject();
	}
	void ProjectileGameObject::Update(double delta_time) {
		SetPosition(GetPosition() + speed_ * GetBearing() * (float)delta_time);
		GameObject::Update(delta_time);
	}
} // namespace game