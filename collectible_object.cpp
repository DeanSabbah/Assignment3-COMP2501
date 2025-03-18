#include "collectible_object.h"
#include "game_object.h"

using namespace game;

CollectibleObject::CollectibleObject() {}

void CollectibleObject::init() {
	parent->setHealth(1);
}