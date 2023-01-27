#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	if (!canClick) {
		state = GuiControlState::DISABLED;
	}
	else {
		state = GuiControlState::NORMAL;
	}

	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {
			
			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d",previousState,state);
				app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}




bool GuiButton::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	/*switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		break;
	}*/

	if (texture != nullptr) {
		SDL_Rect* section;
		switch (state)
		{
		case GuiControlState::DISABLED:
			section = new SDL_Rect{ 0, 0, bounds.w, bounds.h};
			app->render->DrawTexture(texture, bounds.x, bounds.y, section);
			break;
		case GuiControlState::NORMAL:
			section = new SDL_Rect{ bounds.w, 0, bounds.w, bounds.h };
			app->render->DrawTexture(texture, bounds.x, bounds.y, section);
			break;
		case GuiControlState::FOCUSED:
			section = new SDL_Rect{ bounds.w * 2, 0, bounds.w, bounds.h };
			app->render->DrawTexture(texture, bounds.x, bounds.y, section);
			break;
		case GuiControlState::PRESSED:
			section = new SDL_Rect{ bounds.w * 3, 0, bounds.w, bounds.h };
			app->render->DrawTexture(texture, bounds.x, bounds.y, section);
			break;
		}
	}

	switch (state)
	{
	case GuiControlState::DISABLED:
		app->render->DrawText(text.GetString(), bounds.x + 2, bounds.y, bounds.w - 4, bounds.h - 4, { 255,255,255 });
		break;
	case GuiControlState::NORMAL:
		app->render->DrawText(text.GetString(), bounds.x + 2, bounds.y, bounds.w - 4, bounds.h - 4, { 255,255,255 });
		break;
	case GuiControlState::FOCUSED:
		app->render->DrawText(text.GetString(), bounds.x + 2, bounds.y, bounds.w - 4, bounds.h - 4, { 255,255,255 });
		break;
	case GuiControlState::PRESSED:
		app->render->DrawText(text.GetString(), bounds.x + 2, bounds.y + 4, bounds.w - 4, bounds.h - 4, { 255,255,255 });
		break;
	}

	return false;
}

bool GuiButton::DrawDebug(Render* render)
{
	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 200, 200, 200, 100, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(bounds, 0, 0, 255, 100, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(bounds, 255, 0, 0, 100, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(bounds, 0, 255, 0, 100, true, false);
		break;
	}
	return false;
}
