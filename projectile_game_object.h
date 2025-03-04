#ifndef PROJECTILE_GAME_OBJECT_H_
#define PROJECTILE_GAME_OBJECT_H_

#include "game_object.h"
#include "collider_object.h"
#include "timer.h"

namespace game {
	class ProjectileGameObject : public GameObject, public ColliderObject {
	public:
		ProjectileGameObject(const glm::vec3& position, glm::vec3& bearing, Geometry* geom, Shader* shader, GLuint texture, glm::vec2 scale, const float speed, const int damage, const float time, const float radius, const bool type);
		~ProjectileGameObject();
		void Update(double delta_time) override;
		glm::vec3 ColliderObject::getPosition() const { return position_; }
		int getDamage() const { return damage_; }
	private:
		const float speed_;
		const int damage_;
	};
}

#endif // PROJECTILE_GAME_OBJECT_H_