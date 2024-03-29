#include "WinScreen.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "Log.h"
#include "SDL/include/SDL_scancode.h"

WinScreen::WinScreen(bool startEnabled) : Module(startEnabled) {
	name.Create("win_screen");
}

WinScreen::~WinScreen() {}

bool WinScreen::Awake(pugi::xml_node& config) {
	bgPath = config.child("textures").attribute("texturepath").as_string();
	musicPath = (char*)config.child("audio").attribute("musicpath").as_string();

	return true;
}

// Load assets
bool WinScreen::Start() {
	LOG("Loading background assets");
	bgTexture = app->tex->Load(bgPath);
	app->audio->PlayMusic(musicPath, 1.0f);
	bool ret = true;


	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

// Called each loop iteration
bool WinScreen::PreUpdate()
{
	OPTICK_EVENT();
	return true;
}

bool WinScreen::Update(float dt) {
	OPTICK_EVENT();


	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->scene_title, 90);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT) {
		return false;
	}


	return true;
}

// Update: draw background
bool WinScreen::PostUpdate() {
	OPTICK_EVENT();
	// Draw everything --------------------------------------
	app->render->DrawTexture(bgTexture, 0, 0, NULL);

	return true;
}

bool WinScreen::CleanUp() {
	app->tex->UnLoad(bgTexture);
	bgTexture = nullptr;

	return true;
}