// Interface to make gameobjects collide with each other
#ifndef COLLIDER_OBJECT_H
#define COLLIDER_OBJECT_H

namespace game {
	class ColliderObject {
	public:
		void virtual OnCollision(ColliderObject* other) = 0;
	private:

	};
}

#endif // COLLIDER_OBJECT_H