#include "projectile_object.h"
#include "game_object.h"

using namespace game;

ProjectileObject::ProjectileObject(const float speed, const int damage, const float time) : speed_(speed), damage_(damage), time_(time) {}

ProjectileObject::~ProjectileObject() {
	delete parent->timer;
}

void ProjectileObject::Update(double delta_time) {
	parent->SetPosition(parent->GetPosition() + speed_ * parent->GetBearing() * (float)delta_time);
}

void ProjectileObject::init() {
	parent->SetRotation(glm::atan(parent->GetBearing().y, parent->GetBearing().x));
	parent->timer = new Timer();
	parent->timer->Start(time_);
	parent->setHealth(1);
}