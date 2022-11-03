#include "SceneTitle.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL_scancode.h"

SceneTitle::SceneTitle(bool startEnabled) : Module(startEnabled) {
	name.Create("scene_title");
}

SceneTitle::~SceneTitle() {}

// Load assets
bool SceneTitle::Awake(pugi::xml_node& config) {
	bgTexture = app->tex->Load(config.child("textures").attribute("texturepath").as_string());
	musicPath = config.child("audio").attribute("musicpath").as_string();

	return true;
}

bool SceneTitle::Start() {
	bool ret = true;
	
	app->audio->PlayMusic(musicPath, 1.0f);

	duration = 0;
	


	return ret;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	return true;
}

bool SceneTitle::Update(float dt) {

	bool toSkip = false;
	

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || toSkip) {
		app->fade->FadeToBlack(this, (Module*)app->level_one, 30);
	}

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT) {
		return false;
	}


	++duration;

	return true;
}

// Update: draw background
bool SceneTitle::PostUpdate() {

	app->render->DrawTexture(bgTexture, 0, 0);
	
	return true;
}

bool SceneTitle::CleanUp() {
	app->tex->UnLoad(bgTexture);
	bgTexture = nullptr;
	return true;
}