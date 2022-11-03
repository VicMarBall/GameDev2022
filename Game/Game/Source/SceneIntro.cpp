#include "SceneIntro.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "Log.h"
#include "SDL/include/SDL_scancode.h"

SceneIntro::SceneIntro(bool startEnabled) : Module(startEnabled) {
	name.Create("scene_intro");
}

SceneIntro::~SceneIntro() {}

bool SceneIntro::Awake(pugi::xml_node& config) {
	bgTexture = app->tex->Load(config.child("textures").attribute("texturepath").as_string());
	
	return true;
}

// Load assets
bool SceneIntro::Start() {
	LOG("Loading background assets");

	bool ret = true;

	durationTimer = 0;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool SceneIntro::Update(float dt) {
	durationTimer++;
	
	

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
		durationTimer >= 500) {
		app->fade->FadeToBlack(this, (Module*)app->scene_title, 90);
	}
	

	return true;
}

// Update: draw background
bool SceneIntro::PostUpdate() {
	// Draw everything --------------------------------------
	app->render->DrawTexture(bgTexture, 0, 0, NULL);

	return true;
}

bool SceneIntro::CleanUp() {
	app->tex->UnLoad(bgTexture);
	bgTexture = nullptr;
	return true;
}

