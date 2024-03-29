#include "ModuleFadeToBlack.h"

#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Log.h"

#include "SDL/include/SDL_render.h"

ModuleFadeToBlack::ModuleFadeToBlack() : Module() {
	name.Create("fade");
	
}

ModuleFadeToBlack::~ModuleFadeToBlack() {}

bool  ModuleFadeToBlack::Awake(pugi::xml_node& config) {
	screenRect = { 0, 0, (int)(app->win->GetWidth() * app->win->GetScale()), (int)(app->win->GetHeight() * app->win->GetScale()) };
	return true;
}

bool ModuleFadeToBlack::Start() {
	LOG("Preparing Fade Screen");

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Called each loop iteration
bool ModuleFadeToBlack::PreUpdate()
{
	OPTICK_EVENT();
	return true;
}

bool ModuleFadeToBlack::Update(float dt) {
	OPTICK_EVENT();
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	if (currentStep == Fade_Step::TO_BLACK) {
		++frameCount;
		if (frameCount >= maxFadeFrames) {

			moduleToDisable->Disable();
			// moduleToEnable->Enable();

			currentStep = Fade_Step::LOADING;
		}
	} 
	else if (currentStep == Fade_Step::LOADING) {
		moduleToEnable->Enable();

		currentStep = Fade_Step::FROM_BLACK;
	}
	else if (currentStep == Fade_Step::FROM_BLACK) {
		--frameCount;
		if (frameCount <= 0) {
			currentStep = Fade_Step::NONE;
		}
	}

	return true;
}

bool ModuleFadeToBlack::PostUpdate() {
	OPTICK_EVENT();
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool ModuleFadeToBlack::FadeToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames) {
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE) {
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		this->moduleToEnable = moduleToEnable;
		this->moduleToDisable = moduleToDisable;

		ret = true;
	}

	return ret;
}

bool ModuleFadeToBlack::CleanUp() {

	return true;
}
