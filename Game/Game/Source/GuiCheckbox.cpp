#include "GuiCheckbox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"

GuiCheckbox::GuiCheckbox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	if (bounds.w > bounds.h) {
		box = { bounds.x, bounds.y, bounds.h, bounds.h };
	}
	else {
		box = { bounds.x, bounds.y, bounds.w, bounds.w };
	}

	audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
}

GuiCheckbox::~GuiCheckbox()
{

}

bool GuiCheckbox::Update(float dt)
{
	if (!canClick) {
		state = GuiControlState::DISABLED;
	}
	else {
		if (checked) {
			state = GuiControlState::SELECTED;
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
		if (mouseX >= bounds.x && mouseX <= bounds.x + box.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + box.h) {

			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
				app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				SwitchCheck();
				NotifyObserver();
			}
		}
	}

	return false;
}




bool GuiCheckbox::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State
	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(box, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(box, 0, 0, 255, 255, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(box, 0, 0, 20, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(box, 0, 255, 0, 255, true, false);
		break;
	case GuiControlState::SELECTED:
		render->DrawRectangle(box, 255, 0, 0, 255, true, false);
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x + box.w, bounds.y, bounds.w, bounds.h, { 0, 0, 0 });

	return false;
}

bool GuiCheckbox::DrawDebug(Render* render)
{
	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(box, 200, 200, 200, 100, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(box, 0, 0, 255, 100, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(box, 0, 0, 20, 100, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(box, 0, 255, 0, 100, true, false);
		break;
	case GuiControlState::SELECTED:
		render->DrawRectangle(box, 255, 0, 0, 100, true, false);
		break;
	}

	return false;
}

void GuiCheckbox::SwitchCheck()
{
	checked = !checked;
}

void GuiCheckbox::SetCheck(bool state)
{
	checked = state;
}
