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
	bgPath =config.child("textures").attribute("texturepath").as_string();
	
	return true;
}

// Load assets
bool SceneIntro::Start() {
	LOG("Loading background assets");
	bgTexture = app->tex->Load(bgPath);
	bool ret = true;

	durationTimer = 0;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}

bool SceneIntro::Update(float dt) {
	durationTimer++;
	
	

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
		durationTimer >= 150) {
		app->fade->FadeToBlack(this, (Module*)app->scene_title, 90);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT) {
		return false;
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

