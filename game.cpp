#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>

#include <path_config.h>

#include "sprite.h"
#include "shader.h"
#include "player_game_object.h"
#include "collectible_game_object.h"
#include "enemy_game_object.h"
#include "game.h"
#include <time.h>

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Game Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;


void Game::SetupGameWorld(void)
{
    srand(time(NULL));

    // Setup the game world

    // **** Load all the textures that we will need
    //
    // Declare all the textures here
    std::vector<std::string> textures;
    enum { tex_red_ship = 0,
           tex_green_ship = 1,
           tex_blue_ship = 2,
           tex_stars = 3,
           tex_orb = 4,
           explosion = 5,
           yellow_orb = 6,
           invincible = 7};
    textures.push_back("/textures/Ship_4.png"); 
    textures.push_back("/textures/Ship_2.png"); 
    textures.push_back("/textures/Ship_5.png");
    textures.push_back("/textures/stars.png");
    textures.push_back("/textures/orb.png");
    textures.push_back("/textures/Explosion.png");
    textures.push_back("/textures/orb_yellow.png");
	textures.push_back("/textures/Player_invincible.png");
    // Load textures
    LoadTextures(textures);


    // **** Setup all the game objects in the world

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_red_ship]));
    float pi_over_two = glm::pi<float>() / 2.0f;
    game_objects_[0]->SetRotation(pi_over_two);

    // Setup other objects
    game_objects_.push_back(new EnemyGameObject(glm::vec3(-1.0f, 1.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_green_ship]));
    game_objects_[1]->SetRotation(pi_over_two);
    game_objects_.push_back(new EnemyGameObject(glm::vec3(1.0f, -0.5f, 0.0f), sprite_, &sprite_shader_, tex_[tex_blue_ship]));
    game_objects_[2]->SetRotation(pi_over_two);
	// Setup collectible objects in random locations
	for (int i = 0; i < 10; i++) {
		float x = (rand() % 8) - 4;
		float y = (rand() % 7) - 3.5;
		game_objects_.push_back(new CollectibleGameObject(glm::vec3(x, y, 0.0f), sprite_, &sprite_shader_, tex_[yellow_orb]));
		game_objects_[3 + i]->SetRotation(pi_over_two);
	}

    // Setup background
    // In this specific implementation, the background is always the
    // last object
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_stars]);
    background->SetScale(glm::vec2(12.0, 12.0));
    game_objects_.push_back(background);

	spawn_timer = new Timer();
}


void Game::DestroyGameWorld(void)
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }
}


void Game::HandleControls(double delta_time)
{
    // Get player game object
    GameObject *player = game_objects_[0];
    // Get current position and angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetRotation();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();
    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = delta_time*500.0;
    float motion_increment = 0.001*speed;
    float angle_increment = (glm::pi<float>() / 1800.0f)*speed;

    // Check for player input and make changes accordingly
    // Only if the player is not dying
    if (!player->isDying) {
        if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
            player->SetPosition(curpos + motion_increment * dir);
        }
        if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
            player->SetPosition(curpos - motion_increment * dir);
        }
        if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
            player->SetRotation(angle - angle_increment);
        }
        if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
            player->SetRotation(angle + angle_increment);
        }
        if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS) {
            player->SetPosition(curpos - motion_increment * player->GetRight());
        }
        if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
            player->SetPosition(curpos + motion_increment * player->GetRight());
        }
        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window_, true);
        }
    }
}


void Game::Update(double delta_time)
{
	// If spawn timer is finished, spawn a new object in random location
    if (spawn_timer->Finished() || !spawn_timer->Running()) {
        SpawnObject();
	}
    // Casts the current game object to a player object, allows use of Player specific functions
    PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(game_objects_[0]);
    // Update all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

       /* if (current_game_object == game_objects_[0]) {
			current_game_object->printLocation();
        }*/

        // Update the current game object
        current_game_object->Update(delta_time);
		// Check if the object is dying
        if (current_game_object->isDying) {
            // If the object is dying, check if the timer has finished
            if (current_game_object->timer->Finished()) {
				// If player dies, comment "Game over" and close the window
                if (current_game_object == player) {
                    std::cout << "Game over" << std::endl;
                    glfwSetWindowShouldClose(window_, true);
                }
				// If the timer has finished, remove the object from the game world
                game_objects_.erase(game_objects_.begin() + i);
                // Deletes dead object from memory
                delete current_game_object;
                // Makes sure that we don't call the deleted object by skipping the rest of the loop
                continue;
            }
        }

        // Check for collision with other game objects
        // Note the loop bounds: we avoid testing the last object since
        // it's the background covering the whole game world
        for (int j = i + 1; j < (game_objects_.size()-1); j++) {
            GameObject* other_game_object = game_objects_[j];
			// Evalute if the other game object is an enemy object
            EnemyGameObject* enemy = dynamic_cast<EnemyGameObject*>(other_game_object);

            // Compute distance between object i and object j
            float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());
            // If distance is below a threshold, we have a collision
            if (current_game_object == player) {
                if (distance < 0.8f) {
                    // Checks to see if the object that it is currently colliding with the player is new
                    if (!other_game_object->isDying && current_game_object->prev_collider != other_game_object) {
						if (dynamic_cast<CollectibleGameObject*>(other_game_object)) {
							// If the object is a collectible object, collect it
                            player->collect();
							// Remove the object from the game world
							game_objects_.erase(game_objects_.begin() + j);
                            delete other_game_object;
						}
						else if(!player->isDying && enemy){
							// Deals damage to the player and the object that the player collided with
							current_game_object->prev_collider = other_game_object;
							other_game_object->hurt();
                            // Only damage player if they are not invincible
                            if (!player->is_invincible())
    							current_game_object->hurt();
						}
                    }
                }
				else if (enemy) {
					// If the object is an enemy object, update the player's position
					// Way to only change state on enter/exit of range?
                    enemy->updatePlayerPos(player->GetPosition());
                    if (distance < 2.0f)
						enemy->setState(true);
                    else
						enemy->setState(false);
				}
                // Resets the prev collider for the player if they leave the objects collision area
                // This is to ensure that only 1 health is removed per collision
                else if(other_game_object == player->prev_collider)
                    current_game_object->prev_collider = nullptr;
            }
        }
    }
}


void Game::Render(void){

    // Clear background
    glClearColor(viewport_background_color_g.r,
                 viewport_background_color_g.g,
                 viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix;
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    } else {
        float aspect_ratio = ((float) height)/((float) width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f/aspect_ratio, 1.0f));
    }

    // Set view to zoom out, centered by default at 0,0
    float camera_zoom = 0.25f;
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));
    glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix;

    // Render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        game_objects_[i]->Render(view_matrix, current_time_);
    }
}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    spawn_timer = new Timer();
	spawn_timer->Start(10);
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Handle user input
        HandleControls(delta_time);

        // Update all the game objects
        Update(delta_time);

        // Render all the game objects
        Render();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;
}


Game::~Game()
{
    // Free memory used by game world
    DestroyGameWorld();

    // Free rendering resources
    delete sprite_;

    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


void Game::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        std::cout << "Cannot load texture " << fname << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::LoadTextures(std::vector<std::string> &textures)
{
    // Allocate a buffer for all texture references
    int num_textures = textures.size();
    tex_ = new GLuint[num_textures];
    glGenTextures(num_textures, tex_);
    // Load each texture
    for (int i = 0; i < num_textures; i++){
        SetTexture(tex_[i], (resources_directory_g+textures[i]).c_str());
    }
    // Set first texture in the array as default
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}

// Spawns a new object in the game world in a random location
void Game::SpawnObject() {
    //std::cout << "spawing object" << std::endl;
    float pi_over_two = glm::pi<float>() / 2.0f;
    // Randomly generate a location for the new object
	float x = (rand() % 8) - 4;
	float y = (rand() % 7) - 3.5;
	// use either texture 1 or 2
	int texture = rand() % 2 + 1;
	// Create the new object
	EnemyGameObject* new_object = new EnemyGameObject(glm::vec3(x, y, 0.0f), sprite_, &sprite_shader_, tex_[texture]);
    new_object->SetRotation(pi_over_two);
	// Add the new object to the game world
	game_objects_.insert(game_objects_.begin() + game_objects_.size() - 1, new_object);
	// Restart the spawn timer
	spawn_timer->Start(10);
}

} // namespace game
