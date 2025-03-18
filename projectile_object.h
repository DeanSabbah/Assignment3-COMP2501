#pragma once

#include <GL/glew.h>
#include "component.h"
#include "timer.h"
#include <glm/glm.hpp>

using namespace game;

class ProjectileObject : public Component {
public:
	ProjectileObject(const glm::vec3 bearing,  const float speed, const int damage, const float time);
	~ProjectileObject();
	void Update(double delta_time) override;
	void init() override;
	int getDamage() const { return damage_; }
private:
	const float speed_;
	const int damage_;
	const float time_;
};