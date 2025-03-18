#include "player_object.h"
#include "game_object.h"

namespace game {

	/*
		Component that handles the player object
	*/

	PlayerObject::PlayerObject() {}

	PlayerObject::~PlayerObject() {
		delete invincibility_timer;
	}

	// Update function for moving the player object around
	void PlayerObject::Update(double delta_time) {
		if (!parent->isDying())
			// Move player object
			parent->SetPosition(parent->GetPosition() + (float)delta_time * velocity_);
		if (invincibility_timer->Finished()) {
			invincible_ = false;
			parent->setTexture(1);
		}
	}

	void PlayerObject::init() {
		parent->setHealth(3);
		invincibility_timer = new Timer();
		cooldown = new Timer();
	}

	void PlayerObject::collect() {
		collected_objs++;
		if (collected_objs == objs_needed) {
			// Sets player texture to the invincible texture
			parent->setTexture(8);
			invincible_ = true;
			invincibility_timer->Start(invincibility_time);
			collected_objs = 0;
		}
	}

	bool PlayerObject::is_invincible() const {
		return invincible_;
	}
	// case 0 = forward, 1 = backwards, 2 = right, 3 = left
	void PlayerObject::update_velocity(int direction) {
		glm::vec3 dir;
		switch (direction) {
		case 0:
			dir = parent->GetBearing();
			break;
		case 1:
			dir = -1.0f * parent->GetBearing();
			break;
		case 2:
			dir = parent->GetRight();
			break;
		case 3:
			dir = -1.0f * parent->GetRight();
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

	float PlayerObject::get_velocity() const {
		return glm::length(velocity_);
	}

	void PlayerObject::set_velocity(float velocity) {
		velocity_ = velocity * parent->GetBearing();
	}

	void PlayerObject::shoot_projectile() {
		cooldown->Start(0.6f);
	}
}