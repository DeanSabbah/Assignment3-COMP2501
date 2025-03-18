#pragma once

#include "component.h"

namespace game {
	class CollectibleObject : public Component {
	public:
		CollectibleObject();
		void init() override;
	};
}