#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp> 
#define GLEW_STATIC
#include <GL/glew.h>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "shader.h"
#include "geometry.h"
#include "timer.h"
#include "component.h"

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {
        public:
            // Constructor
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, glm::vec2& scale);
            ~GameObject();

			// Add a component to the GameObject
			template <typename T, typename... Args>
            T* AddComponent(Args&&... args) {
                static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
                auto component = std::make_unique<T>(std::forward<Args>(args)...);
                component->setParent(this);
				component->init();
                T* rawPtr = component.get();
                components_[typeid(T)] = std::move(component);
                return rawPtr;
            }

			// Get a component from the GameObject
			template <typename T>
            T* GetComponent() {
                auto it = components_.find(typeid(T));
                return (it != components_.end()) ? dynamic_cast<T*>(it->second.get()) : nullptr;
            }

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time);

            // Renders the GameObject 
            virtual void Render(glm::mat4 view_matrix, double current_time);

            // Getters
            inline glm::vec3 GetPosition(void) const { return position_; }
            inline glm::vec2 GetScale(void) const { return scale_; }
            inline float GetRotation(void) const { return angle_; }

            // Get bearing direction (direction in which the game object
            // is facing)
            glm::vec3 GetBearing(void) const;

            // Get vector pointing to the right side of the game object
            glm::vec3 GetRight(void) const;

			// Get object's ghost status
			bool getGhostMode() const { return ghost_; }

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(glm::vec2& scale) { scale_ = scale; }
            void SetRotation(float angle);

			// Set the object to be a ghost
			void setGhostMode(bool ghost) { ghost_ = ghost; }
			// Set the objects health
			void setHealth(int h) { health = h; }

            void hurt();
			bool isDying() { return _dying; }
            GameObject* prev_collider;
			Timer* timer;

			void setTexture(GLuint w);

			void printLocation();


        protected:
			// Map to store the object's components
            std::unordered_map<std::type_index, std::unique_ptr<Component>> components_;
            int health;
            // Bool to make sure the player is not hurt when they collide with a dying object
            bool _dying = false;
            void die();
            bool ghost_ = false;
            // Object's Transform Variables
            glm::vec3 position_;
            glm::vec2 scale_;
            float angle_;

            // Geometry
            Geometry *geometry_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
