#include "DeathScreen.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "Log.h"
#include "SDL/include/SDL_scancode.h"

DeathScreen::DeathScreen(bool startEnabled) : Module(startEnabled) {
	name.Create("death_screen");
}

DeathScreen::~DeathScreen() {}

bool DeathScreen::Awake(pugi::xml_node& config) {
	bgPath = config.child("textures").attribute("texturepath").as_string();
	musicPath = (char*)config.child("audio").attribute("musicpath").as_string();

	return true;
}

// Load assets
bool DeathScreen::Start() {
	LOG("Loading background assets");
	bgTexture = app->tex->Load(bgPath);
	app->audio->PlayMusic(musicPath, 1.0f);
	bool ret = true;


	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

// Called each loop iteration
bool DeathScreen::PreUpdate()
{
	OPTICK_EVENT();
	return true;
}

bool DeathScreen::Update(float dt) {
	OPTICK_EVENT();


	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		app->fromCheckpoint = true;

		switch (level)
		{
		case 1:
			app->fade->FadeToBlack(this, (Module*)app->level_one, 90);
		case 2:
			app->fade->FadeToBlack(this, (Module*)app->level_two, 90);
		default:
			break;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT) {
		return false;
	}


	return true;
}

// Update: draw background
bool DeathScreen::PostUpdate() {
	OPTICK_EVENT();
	// Draw everything --------------------------------------
	app->render->DrawTexture(bgTexture, 0, 0, NULL);

	return true;
}

bool DeathScreen::CleanUp() {
	app->tex->UnLoad(bgTexture);
	bgTexture = nullptr;

	return true;
}