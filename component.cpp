#include "game_object.h"
#include "component.h"

namespace game {
	GameObject* Component::GetParent() const {
		return parent;
	}
	void Component::setParent(GameObject* parent) {
		this->parent = parent;
	}
}