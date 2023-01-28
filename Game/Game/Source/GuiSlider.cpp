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
		box = { bounds.x - (int)(bounds.h * 0.5f), bounds.y, bounds.h, bounds.h };
	}
	else {
		box = { bounds.x - (int)(bounds.w * 0.5f), bounds.y, bounds.w, bounds.w };
	}

	valueSlider = 0;
}

GuiSlider::~GuiSlider()
{

}

bool GuiSlider::Update(float dt)
{
	OPTICK_EVENT();
	GuiControlState previousState = state;

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


		// I'm inside the limitis of the button
		if (mouseX >= box.x && mouseX <= box.x + box.w &&
			mouseY >= box.y && mouseY <= box.y + box.h) {

			state = GuiControlState::FOCUSED;

			if (previousState == GuiControlState::NORMAL) {
				app->audio->PlayFx(hoverSFX);
			}

			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
				int newValue = valueSlider + (mouseX - mousePreviousX);
				SetValue(newValue);
				NotifyObserver();
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				app->audio->PlayFx(pressSFX);
			}
		}
		else {
			if (state == GuiControlState::PRESSED) {
				int newValue = valueSlider + (mouseX - mousePreviousX);
				SetValue(newValue);
				NotifyObserver();
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
	sliderLine.y = bounds.y + (bounds.h * 0.5f) - 2;
	sliderLine.w = sliderLength;
	sliderLine.h = 4;

	if (texture != nullptr) {
		SDL_Rect* section;
		switch (state)
		{
		case GuiControlState::DISABLED:
			render->DrawRectangle(sliderLine, 200, 200, 200, 200, true, false);
			section = new SDL_Rect{ 0, 0, box.w, box.h };
			app->render->DrawTexture(texture, box.x, box.y, section, false);
			break;
		case GuiControlState::NORMAL:
			render->DrawRectangle(sliderLine, 200, 200, 200, 200, true, false);
			section = new SDL_Rect{ box.w, 0, box.w, box.h };
			app->render->DrawTexture(texture, box.x, box.y, section, false);
			break;
		case GuiControlState::FOCUSED:
			render->DrawRectangle(sliderLine, 200, 200, 200, 200, true, false);
			section = new SDL_Rect{ box.w * 2, 0, box.w, box.h };
			app->render->DrawTexture(texture, box.x, box.y, section, false);
			break;
		case GuiControlState::PRESSED:
			render->DrawRectangle(sliderLine, 200, 200, 200, 200, true, false);
			section = new SDL_Rect{ box.w * 3, 0, box.w, box.h };
			app->render->DrawTexture(texture, box.x, box.y, section, false);
			break;
		}
	}

	SDL_Rect textBounds;
	textBounds.x = bounds.x + sliderLength + (box.w * 0.5f);
	textBounds.y = bounds.y;
	textBounds.w = bounds.w;
	textBounds.h = bounds.h;
	//render->DrawRectangle(textBounds, 0, 0, 0, 255, true, false);
	app->render->DrawText(text.GetString(), textBounds.x, textBounds.y, textBounds.w, textBounds.h, { 0,0,0 });

	return false;
}

bool GuiSlider::DrawDebug(Render* render)
{
	SDL_Rect sliderLine;
	sliderLine.x = bounds.x;
	sliderLine.y = bounds.y + (bounds.h * 0.5f) - 2;
	sliderLine.w = sliderLength;
	sliderLine.h = 4;

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(sliderLine, 200, 200, 200, 100, true, false);
		render->DrawRectangle(box, 200, 200, 200, 100, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(sliderLine, 100, 100, 100, 100, true, false);
		render->DrawRectangle(box, 0, 0, 255, 100, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(sliderLine, 100, 100, 100, 100, true, false);
		render->DrawRectangle(box, 255, 0, 0, 100, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(sliderLine, 100, 100, 100, 100, true, false);
		render->DrawRectangle(box, 0, 255, 0, 100, true, false);
		break;
	}
	return false;
}
