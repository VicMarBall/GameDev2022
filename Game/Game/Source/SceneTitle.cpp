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
	menuPath = config.child("menutexture").attribute("texturepath").as_string();
	return true;
}

bool SceneTitle::Start() {
	bool ret = true;
	
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->audio->PlayMusic(musicPath, 1.0f);
	bgTexture = app->tex->Load(bgPath);
	
	menuBackground = app->tex->Load(menuPath);

	withMenu = false;

	playButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "PLAY", { 100, 100, 100, 50 }, this);

	continueButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "CONTINUE", { 100, 200, 75, 25 }, this);
	
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "SETTINGS", { 100, 250, 75, 25 }, this);
	
	creditsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "CREDITS", { 100, 300, 75, 25 }, this);
	
	exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "EXIT", { 100, 350, 40, 25 }, this);
	
	backToTitle = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "BACK", { 100, 350, 50, 25 }, this);
	backToTitle->TurnOFF();

	musicVolumeSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 9, "MUSIC", { 200, 150, 50, 25 }, this, 128);
	musicVolumeSlider->TurnOFF();
	musicVolumeSlider->SetValue(app->audio->GetVolumeMusic());

	SFXVolumeSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 10, "SFX", { 200, 200, 30, 25 }, this, 128);
	SFXVolumeSlider->TurnOFF();
	SFXVolumeSlider->SetValue(app->audio->GetSFXVolume());

	fullScreenCheckbox = (GuiCheckbox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 11, "FULLSCREEN", { 200, 250, 100, 25 }, this);
	fullScreenCheckbox->TurnOFF();

	VSyncCheckbox = (GuiCheckbox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 12, "VSYNC", { 200, 300, 50, 25 }, this);
	VSyncCheckbox->TurnOFF();

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

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		if (app->guiManager->debug) {
			app->guiManager->debug = false;
		}
		else {
			app->guiManager->debug = true;
		}
	}

	switch (stateScene)
	{
	case SceneTitle::TITLE:
		

		break;
	case SceneTitle::PLAY:
		app->fade->FadeToBlack(this, (Module*)app->level_one, 30);

		break;
	case SceneTitle::CONTINUE:
		// load save_file
		app->LoadGameRequest();

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
	if (withMenu) {
		app->render->DrawTexture(menuBackground, 0, 0);
	}
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

	app->guiManager->Clear(SFXVolumeSlider);
	app->guiManager->Clear(backToTitle);
	app->guiManager->Clear(VSyncCheckbox);
	app->guiManager->Clear(fullScreenCheckbox);
	app->guiManager->Clear(musicVolumeSlider);

	return true;
}

bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{
	// 1 == PLAY
	// 2 == CONTINUE
	// 3 == SETTINGS
	// 4 == CREDITS
	// 5 == EXIT

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

		playButton->TurnOFF();
		continueButton->TurnOFF();
		settingsButton->TurnOFF();
		creditsButton->TurnOFF();
		exitButton->TurnOFF();

		SFXVolumeSlider->TurnON();
		backToTitle->TurnON();
		VSyncCheckbox->TurnON();
		fullScreenCheckbox->TurnON();
		musicVolumeSlider->TurnON();

		withMenu = true;

		break;
	case 4:
		stateScene = CREDITS;

		playButton->TurnOFF();
		continueButton->TurnOFF();
		settingsButton->TurnOFF();
		creditsButton->TurnOFF();
		exitButton->TurnOFF();

		backToTitle->TurnON();

		withMenu = true;

		break;
	case 5:
		stateScene = EXIT;

		break;
	case 6:

		break;
	case 7:
		stateScene = TITLE;

		playButton->TurnON();
		continueButton->TurnON();
		settingsButton->TurnON();
		creditsButton->TurnON();
		exitButton->TurnON();

		SFXVolumeSlider->TurnOFF();
		backToTitle->TurnOFF();
		VSyncCheckbox->TurnOFF();
		fullScreenCheckbox->TurnOFF();
		musicVolumeSlider->TurnOFF();

		withMenu = false;

		break;
	case 8:
		LOG("HEYYY :DDD");
	case 9:
		app->audio->SetVolumeMusic(((GuiSlider*)control)->valueSlider);
		break;
	case 10:
		app->audio->SetSFXVolume(((GuiSlider*)control)->valueSlider);
		break;
	default:
		break;
	}

	return false;
}