#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include "timer.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
			// Constructor
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);
			// Destructor
            ~PlayerGameObject();

            void Update(double delta_time) override;
			// Function to collect objects
            void collect();
			// Returns if the player is invincible
			bool is_invincible() const;
			// Timer for invincibility
			Timer* invincibility_timer;

        private:
            bool invincible_= false;
			int collected_objs = 0;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
