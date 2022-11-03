#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Point.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

class SceneIntro : public Module {
public:
	//Constructor
	SceneIntro(bool startEnabled);

	//Destructor
	~SceneIntro();

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

	bool CleanUp();
public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;

	unsigned short durationTimer = 0;
private:

};

#endif