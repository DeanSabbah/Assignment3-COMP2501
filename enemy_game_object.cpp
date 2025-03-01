#include "enemy_game_object.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp> 

using namespace game;

EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture) : GameObject(position, geom, shader, texture) {
	// random height and width for the patrol area
	height_ = (rand() % 20 - 10) / 10.0f;
	radius_ = (rand() % 20 - 10) / 10.0f;
	// center is position -/+ height and width
	if (rand() % 2) {
		center_ = glm::vec3(position.x + radius_, position.y + height_, 0.0f);
	}
	else {
		center_ = glm::vec3(position.x - radius_, position.y - height_, 0.0f);
	}
	move_timer = new Timer();
}

EnemyGameObject::~EnemyGameObject() {
	delete move_timer;
	GameObject::~GameObject();
}

void EnemyGameObject::Update(double delta_time) {
	// If the enemy is close enough to the player, it will intercept the player
	// Will not move the enemy if they are dying
	if (!isDying) {
		if (state_)
			intercept(delta_time);
		else
			patrol(delta_time);
	}

	// Update function for moving the enemy object around
	GameObject::Update(delta_time);
}

// Update the player position stored in the enemy object and calculate the distance between the player and the enemy
void EnemyGameObject::updatePlayerPos(glm::vec3 player_pos) {
	player_pos_ = player_pos;
	distance_ = glm::distance(player_pos_, position_);
}

// Set the state of the enemy object
void EnemyGameObject::setState(bool state) {
	state_ = state;
}

bool EnemyGameObject::getState() const {
	return state_;
}

void EnemyGameObject::patrol(double delta_time) {
	glm::vec3 new_pos;
	// Get the new position
	// Uses the current time to move the enemy in an elliptical motion
	new_pos.x = center_.x + radius_ * cos(glfwGetTime());
	new_pos.y = center_.y + height_ * sin(glfwGetTime());
	// Set rotation to forward
	SetRotation(atan2(new_pos.y - position_.y, new_pos.x - position_.x));
	// Set the new position
	SetPosition(new_pos);
}

void EnemyGameObject::intercept(double delta_time) {
	if (move_timer->Finished()) {
		// Get the direction to the player
		velocity_= glm::normalize(player_pos_ - position_);
		// Start the timer for the next direction update
		move_timer->Start(2);
		// Set rotation to forward
		SetRotation(atan2(velocity_.y, velocity_.x));
	}
	// Set the new position
	SetPosition(position_ + (velocity_ * 0.001f * (float)delta_time * 420.0f));
	// Update the distance to the player, so that when the enemy is far enough, it will patrol around it's current locaiton
	center_ = position_;

}