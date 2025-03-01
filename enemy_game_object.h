#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"
#include "timer.h"

namespace game {
	class EnemyGameObject : public GameObject {
		public:
			// Constructor
			EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
			// Destructor
			~EnemyGameObject();
			void Update(double delta_time) override;
			void updatePlayerPos(glm::vec3 player_pos);
			void setState(bool state);
			bool getState() const;
		private:
			// Object's velcity
			glm::vec3 velocity_;
			// Enemy states
			void patrol(double delta_time);
			void intercept(double delta_time);
			// Player position
			glm::vec3 player_pos_;
			// Distance from player
			float distance_ = NULL;

			// Boolean switch for movement state
			bool state_ = false;
			// Center to patrol
			glm::vec3 center_;
			// Patrol height
			float height_ = NULL;
			// Patrol radius
			float radius_ = NULL;

			Timer* move_timer;
	};
}

#endif // ENEMY_GAME_OBJECT_H_