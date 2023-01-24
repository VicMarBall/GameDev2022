#include "SceneTitle.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "GuiManager.h"
#include "SDL/include/SDL_scancode.h"

SceneTitle::SceneTitle(bool startEnabled) : Module(startEnabled) {
	name.Create("scene_title");
}

SceneTitle::~SceneTitle() {}

// Load assets
bool SceneTitle::Awake(pugi::xml_node& config) {
	bgPath = config.child("textures").attribute("texturepath").as_string();
	musicPath = config.child("audio").attribute("musicpath").as_string();
	return true;
}

bool SceneTitle::Start() {
	bool ret = true;
	
	app->audio->PlayMusic(musicPath, 1.0f);
	bgTexture = app->tex->Load(bgPath);
	
	playButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "PLAY", { 100, 100, 100, 50 }, app->scene_title);

	continueButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "CONTINUE", { 100, 200, 75, 25 }, app->scene_title);
	
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "SETTINGS", { 100, 250, 75, 25 }, app->scene_title);
	
	creditsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "CREDITS", { 100, 300, 75, 25 }, app->scene_title);
	
	exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "EXIT", { 100, 350, 50, 25 }, app->scene_title);
	
	stateScene = TITLE;

	return ret;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	return true;
}

bool SceneTitle::Update(float dt) {

	bool toSkip = false;

	switch (stateScene)
	{
	case SceneTitle::TITLE:

		break;
	case SceneTitle::PLAY:
		app->fade->FadeToBlack(this, (Module*)app->level_one, 30);

		break;
	case SceneTitle::CONTINUE:
		// load save_file

		break;
	case SceneTitle::SETTINGS:
		// hide all ui and show only settings

		break;
	case SceneTitle::CREDITS:
		// credits

		break;
	case SceneTitle::EXIT:

		return false;
		break;
	default:
		break;
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || toSkip) {
		app->fade->FadeToBlack(this, (Module*)app->level_one, 30);
	}

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT) {
		return false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->level_one, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->level_two, 0);
	}

	return true;
}

// Update: draw background
bool SceneTitle::PostUpdate() {

	app->render->DrawTexture(bgTexture, 0, 0);
	app->guiManager->Draw();

	return true;
}

bool SceneTitle::CleanUp() {
	app->tex->UnLoad(bgTexture);
	bgTexture = nullptr;
	app->guiManager->Clear(playButton);
	app->guiManager->Clear(continueButton);
	app->guiManager->Clear(settingsButton);
	app->guiManager->Clear(creditsButton);
	app->guiManager->Clear(exitButton);


	return true;
}

bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{

	switch (control->id)
	{
	case 1:
		stateScene = PLAY;

		break;
	case 2:
		stateScene = CONTINUE;

		break;
	case 3:
		stateScene = SETTINGS;

		break;
	case 4:
		stateScene = CREDITS;

		break;
	case 5:
		stateScene = EXIT;

		break;
	default:
		break;
	}

	return false;
}