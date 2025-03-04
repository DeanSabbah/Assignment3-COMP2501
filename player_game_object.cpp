#include "player_game_object.h"
#include <iostream>

namespace game {

	/*
		PlayerGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	PlayerGameObject::PlayerGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, glm::vec2 scale, const float radius) : GameObject(position, geom, shader, texture, scale), ColliderObject(radius) {
		health = 3;
		invincibility_timer = new Timer();
		cooldown = new Timer();
	}

	PlayerGameObject::~PlayerGameObject() {
		delete invincibility_timer;
		GameObject::~GameObject();
	}

	// Update function for moving the player object around
	void PlayerGameObject::Update(double delta_time) {
		// Move player object
		position_ += (float)delta_time * velocity_;
		if (invincibility_timer->Finished()) {
			invincible_ = false;
			setTexture(1);
		}
		GameObject::Update(delta_time);
	}

	void PlayerGameObject::collect() {
		collected_objs++;
		if (collected_objs == objs_needed) {
			// Sets player texture to the invincible texture
			setTexture(8);
			invincible_ = true;
			invincibility_timer->Start(invincibility_time);
			collected_objs = 0;
		}
	}

	bool PlayerGameObject::is_invincible() const {
		return invincible_;
	}
	// case 0 = forward, 1 = backwards, 2 = right, 3 = left
	void PlayerGameObject::update_velocity(int direction) {
		glm::vec3 dir;
		switch (direction) {
			case 0:
				dir = GetBearing();
				break;
			case 1:
				dir = -1.0f * GetBearing();
				break;
			case 2:
				dir = GetRight();
				break;
			case 3:
				dir = -1.0f * GetRight();
				break;
		}
		if (glm::length(velocity_) < max_velocity_)
			velocity_ += acceleration_ * dir;
		else {
			glm::vec3 targetDir = glm::normalize(dir);
			glm::vec3 currentDir = glm::normalize(velocity_);
			float dot = glm::dot(currentDir, targetDir);
			dot = glm::clamp(dot, -1.0f, 1.0f);
			float angle = glm::acos(dot);
			float stepAngle = glm::radians(1.0f);
			if (angle < stepAngle) stepAngle = angle;
			float cross = currentDir.x * targetDir.y - currentDir.y * targetDir.x;
			glm::vec3 rotationAxis = glm::cross(currentDir, targetDir);
			// Check if the rotation axis is parallel to the current direction
			// If so, choose an arbitrary perpendicular axis
			if (glm::length(rotationAxis) < 1e-6) {
				if (dot < 0) {
					// If opposite direction, rotate 180° around an arbitrary perpendicular axis
					rotationAxis = glm::vec3(1, 0, 0); // Choose any axis perpendicular to currentDir
					if (fabs(currentDir.x) > 0.9f)
						rotationAxis = glm::vec3(0, 1, 0);
				}
				// If the direction is the same, do nothing
				else {
					return;
				}
			}
			velocity_ = glm::rotate(currentDir, stepAngle, glm::normalize(rotationAxis)) * max_velocity_;
		}
	}

	float PlayerGameObject::get_velocity() const {
		return glm::length(velocity_);
	}

	void PlayerGameObject::set_velocity(float velocity) {
		velocity_ = velocity * GetBearing();
	}

	void PlayerGameObject::shoot_projectile() {
		cooldown->Start(0.6f);
	}
}
