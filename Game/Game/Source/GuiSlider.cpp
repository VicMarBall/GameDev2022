#include "GuiSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, int sliderLength) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	this->sliderLength = sliderLength;
	
	if (bounds.w > bounds.h) {
		box = { bounds.x - (int)(bounds.h * 0.5f), bounds.y - (int)(bounds.h * 0.5f), bounds.h, bounds.h };
	}
	else {
		box = { bounds.x - (int)(bounds.w * 0.5f), bounds.y - (int)(bounds.w * 0.5f), bounds.w, bounds.w };
	}

	valueSlider = 0;

	audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
}

GuiSlider::~GuiSlider()
{

}

bool GuiSlider::Update(float dt)
{
	if (!canClick) {
		state = GuiControlState::DISABLED;
	}
	else {
		if (state == GuiControlState::PRESSED && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KeyState::KEY_UP) {
			state == GuiControlState::PRESSED;
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= box.x && mouseX <= box.x + box.w &&
			mouseY >= box.y && mouseY <= box.y + box.h) {

			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
				app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
				int newValue = valueSlider + (mouseX - mousePreviousX);
				SetValue(newValue);
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			if (state == GuiControlState::PRESSED) {
				int newValue = valueSlider + (mouseX - mousePreviousX);
				SetValue(newValue);
			}
			else {
				state = GuiControlState::NORMAL;
			}
		}
		mousePreviousX = mouseX;
		mousePreviousY = mouseY;
		LOG("VALUE: %d", valueSlider);
	}

	return false;
}




bool GuiSlider::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	SDL_Rect sliderLine;
	sliderLine.x = bounds.x;
	sliderLine.y = bounds.y + (bounds.h * 0.5f);
	sliderLine.w = bounds.w;
	sliderLine.h = 10;

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(sliderLine, 200, 200, 200, 200, true, false);
		render->DrawRectangle(box, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(sliderLine, 100, 100, 100, 255, true, false);
		render->DrawRectangle(box, 0, 0, 255, 255, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(sliderLine, 100, 100, 100, 255, true, false);
		render->DrawRectangle(box, 0, 0, 20, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(sliderLine, 100, 100, 100, 255, true, false);
		render->DrawRectangle(box, 0, 255, 0, 255, true, false);
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, { 0,0,0 });

	return false;
}