#ifndef __LEVELTRANSITION_H__
#define __LEVELTRANSITION_H__

#include "Module.h"
#include "Point.h"
#include "Log.h"
#include "SDL/include/SDL.h"

#define MAX_ASSETS_TITLE 13

struct SDL_Texture;

class LevelTransition : public Module {
public:
	// Constructor
	LevelTransition(bool startEnabled);

	// Destructor
	~LevelTransition();

	bool Awake(pugi::xml_node& config) override;

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called before all Updates
	bool PreUpdate();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

	// Disables the player, the enemies and the powerups.
	bool CleanUp();

public:
	// Scene duration
	uint duration;

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture;
	const char* bgPath;
	const char* musicPath;

	// Textures & Animations

};

#endif // __LEVELTRANSITION_H__
