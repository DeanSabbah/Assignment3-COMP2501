#include "player_game_object.h"
#include <iostream>

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture) : GameObject(position, geom, shader, texture) {
	health = 3;
	invincibility_timer = new Timer();
}

PlayerGameObject::~PlayerGameObject() {
	delete invincibility_timer;
	GameObject::~GameObject();
}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {
	if (invincibility_timer->Finished()) {
		invincible_ = false;
		setTexture(1);
	}
	GameObject::Update(delta_time);
}

void PlayerGameObject::collect() {
	collected_objs++;
	if (collected_objs == 10) {
		// Sets player texture to the invincible texture
		setTexture(8);
		invincible_ = true;
		invincibility_timer->Start(10);
		collected_objs = 0;
	}
}

bool PlayerGameObject::is_invincible() const{
	return invincible_;
}

} // namespace game
