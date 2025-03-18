#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "component.h"
#include "timer.h"

namespace game {
	class EnemyObject : public Component {
	public:
		EnemyObject();
		~EnemyObject();
		void Update(double delta_time) override;
		void init() override;
		void updatePlayerPos(glm::vec3 player_pos);
		void setState(bool state);
		bool getState() const;
		float getSpeed() const;
	private:
		// Object's velcity
		glm::vec3 velocity_;
		// Object's speed
		const float speed_ = 420.0f;
		// Enemy states
		void patrol(double delta_time);
		void intercept(double delta_time);
		// Player position
		glm::vec3 player_pos_;
		// Distance from player
		float distance_ = NULL;
		// Boolean switch for movement state
		// Can change to int later for more states
		bool state_ = false;
		// Center to patrol
		glm::vec3 center_;
		// Patrol height
		float height_ = NULL;
		// Patrol radius
		float radius_ = NULL;

		Timer* move_timer;

	};
}