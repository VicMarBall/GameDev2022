#include "LevelTransition.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "Log.h"
#include "SDL/include/SDL_scancode.h"

LevelTransition::LevelTransition(bool startEnabled) : Module(startEnabled) {
	name.Create("level_transition");
}

LevelTransition::~LevelTransition() {}

bool LevelTransition::Awake(pugi::xml_node& config) {
	bgPath = config.child("textures").attribute("texturepath").as_string();

	return true;
}

// Load assets
bool LevelTransition::Start() {
	LOG("Loading background assets");
	bgTexture = app->tex->Load(bgPath);
	bool ret = true;


	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

// Called each loop iteration
bool LevelTransition::PreUpdate()
{
	return true;
}

bool LevelTransition::Update(float dt) {



	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->scene_title, 90);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT) {
		return false;
	}


	return true;
}

// Update: draw background
bool LevelTransition::PostUpdate() {
	// Draw everything --------------------------------------
	app->render->DrawTexture(bgTexture, 0, 0, NULL);

	return true;
}

bool LevelTransition::CleanUp() {
	app->tex->UnLoad(bgTexture);
	bgTexture = nullptr;
	return true;
}