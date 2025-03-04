Changes for part 1 can be found in the textures folder (new textures), the collectable_game_object.cpp/h files and the game.cpp (where textures are loaded and initial game objects are spawned) file
Changes for part 2/3 can be found in the enemy_game_object.cpp/h files and the game.cpp file (where initial game objects are spawned + where new objects are spawned)

BUGS:
Enemies will snap to their new eliptical patrol path because the center of it is it's current location (when intercepting)
Enemies will continue to follow you after they die (unitl they are despawned), they will not damage you though
If the player collects the 10th collectable object after they die, their sprite will change to the invinsible sprite
The player can still move after they die (until the game ends)

SOURCES:
	Spaceship sprites: https://opengameart.org/content/spaceships-32x32
	Licence: CC-BY-40
	
	Explosion sprite: https://commons.wikimedia.org/wiki/File:Explosion-155624_icon.svg
	Licence: CC0

	Orbs sprite: https://opengameart.org/content/pixel-orbs
	Licence: CC0
	
	Fireball sprite: https://nyknck.itch.io/pixelarteffectfx017
	No licence

	Sun with shades: https://opengameart.org/content/sun-with-shades
	Licence: CC0