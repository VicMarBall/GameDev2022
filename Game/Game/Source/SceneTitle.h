#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Module.h"
#include "Point.h"
#include "Log.h"
#include "SDL/include/SDL.h"

#include "GuiButton.h"
#include "GuiCheckbox.h"

#define MAX_ASSETS_TITLE 13

struct SDL_Texture;

class SceneTitle : public Module {
public:
	// Constructor
	SceneTitle(bool startEnabled);

	// Destructor
	~SceneTitle();

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

	bool OnGuiMouseClickEvent(GuiControl* control);

	enum StateTitleScene {
		TITLE,
		PLAY,
		CONTINUE,
		SETTINGS,
		CREDITS,
		EXIT
	};

public:

	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture;
	const char* bgPath;
	const char* musicPath;

	StateTitleScene stateScene;

	// main title ui
	GuiButton* playButton;
	GuiButton* continueButton;
	GuiButton* settingsButton;
	GuiButton* creditsButton;
	GuiButton* exitButton;

	// setting ui
	// TODO Sliders
	GuiButton* soundButton;
	GuiCheckbox* checkboxTest;
	GuiButton* backFromSettingsButton;

	// Textures & Animations
	
};

#endif // __SCENETITLE_H__
