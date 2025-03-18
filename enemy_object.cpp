#include "enemy_object.h"
#include "game_object.h"

using namespace game;

EnemyObject::EnemyObject() {}

EnemyObject::~EnemyObject() {
	delete move_timer;
}

void EnemyObject::Update(double delta_time) {
	// If the enemy is close enough to the player, it will intercept the player
	// Will not move the enemy if they are dying
	if (!parent->isDying()) {
		if (state_)
			intercept(delta_time);
		else
			patrol(delta_time);
	}
}

void EnemyObject::init() {
	// random height and width for the patrol area
	height_ = (rand() % 20 - 10) / 10.0f;
	radius_ = (rand() % 20 - 10) / 10.0f;
	// center is position -/+ height and width
	if (rand() % 2) {
		center_ = glm::vec3(parent->GetPosition().x + radius_, parent->GetPosition().y + height_, 0.0f);
	}
	else {
		center_ = glm::vec3(parent->GetPosition().x - radius_, parent->GetPosition().y - height_, 0.0f);
	}
	move_timer = new Timer();
}

// Update the player position stored in the enemy object and calculate the distance between the player and the enemy
void EnemyObject::updatePlayerPos(glm::vec3 player_pos) {
	player_pos_ = player_pos;
	distance_ = glm::distance(player_pos_, parent->GetPosition());
}

void EnemyObject::setState(bool state) {
	state_ = state;
}

bool EnemyObject::getState() const {
	return state_;
}

float EnemyObject::getSpeed() const {
	return speed_;
}

void EnemyObject::patrol(double delta_time) {
	glm::vec3 new_pos;
	// Get the new position
	// Uses the current time to move the enemy in an elliptical motion
	new_pos.x = center_.x + radius_ * cos(glfwGetTime());
	new_pos.y = center_.y + height_ * sin(glfwGetTime());
	// Set rotation to forward
	parent->SetRotation(atan2(new_pos.y - parent->GetPosition().y, new_pos.x - parent->GetPosition().x));
	// Set the new position
	parent->SetPosition(new_pos);
}

void EnemyObject::intercept(double delta_time) {
	if (move_timer->Finished()) {
		// Get the direction to the player
		velocity_ = glm::normalize(player_pos_ - parent->GetPosition());
		// Start the timer for the next direction update
		move_timer->Start(2);
		// Set rotation to forward
		parent->SetRotation(atan2(velocity_.y, velocity_.x));
	}
	// Set the new position
	parent->SetPosition(parent->GetPosition() + (velocity_ * 0.001f * (float)delta_time * speed_));
	// Update the distance to the player, so that when the enemy is far enough, it will patrol around it's current locaiton
	center_ = parent->GetPosition();
}