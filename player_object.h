#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "component.h"
#include "timer.h"

namespace game {

	// Inherits from GameObject
	class PlayerObject : public Component {

	public:
		PlayerObject();
		~PlayerObject();
		void Update(double delta_time) override;
		void init() override;
		// Function to collect objects
		void collect();
		// Returns if the player is invincible
		bool is_invincible() const;
		// Timer for invincibility
		Timer* invincibility_timer;
		// Update velocity
		// case 0 = forward, 1 = baclward, 2 = right, 3 = left
		void update_velocity(int direction);
		// Get the velocity of the player
		float get_velocity() const;
		// Set the velocity of the player
		void set_velocity(float velocity);
		// Shoots a projectile
		void shoot_projectile();
		// Returns cooldown status
		bool cooling_down() { return !cooldown->Finished(); }


	private:
		// Maximum velocity of the player
		const float max_velocity_ = 2.0f;
		// Acceleration of the player
		const float acceleration_ = 0.05f;
		// Velocity of the player
		glm::vec3 velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
		// Number of objects needed to collect to become invincible
		const int objs_needed = 10;
		// Number of objects collected
		int collected_objs = 0;
		// Amount of time the player is invincible
		const float invincibility_time = 10.0f;
		// Bool to check if the player is invincible
		bool invincible_ = false;
		// Cooldown Timer
		Timer* cooldown;

	}; // class PlayerGameObject

} // namespace game