#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckbox.h"
#include "GuiImage.h"
#include "GuiText.h"
#include "Audio.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	debug = false;

	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, int sliderLength)
{
	// L15: DONE1: Create a GUI control and add it to the list of controls

	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiButton(id, bounds, text);
		break;
	case GuiControlType::CHECKBOX:
		guiControl = new GuiCheckbox(id, bounds, text);
		break;
	case GuiControlType::SLIDER:
		guiControl = new GuiSlider(id, bounds, text, sliderLength);
		break;
		/*
	case GuiControlType::TOGGLE:
		break;
	case GuiControlType::SLIDERBAR:
		break;
	case GuiControlType::COMBOBOX:
		break;
	case GuiControlType::DROPDOWNBOX:
		break;
	case GuiControlType::INPUTBOX:
		break;
	case GuiControlType::VALUEBOX:
		break;
	case GuiControlType::SPINNER:
		break;
		*/
	case GuiControlType::IMAGE:
		guiControl = new GuiImage(id, bounds);
		break;
	case GuiControlType::TEXT:
		guiControl = new GuiText(id, text, bounds);
		break;
	default:
		break;
	}

	guiControl->id = id;

	//Set the observer
	guiControl->SetObserver(observer);

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{	
	OPTICK_EVENT();
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	// We control how often the GUI is updated to optimize the performance
	if (doLogic == true)
	{
		ListItem<GuiControl*>* control = guiControlsList.start;

		while (control != nullptr)
		{
			control->data->Update(dt);
			control = control->next;
		}

		accumulatedTime = 0.0f;
		doLogic = false;
	}
	
	return true;
}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = guiControlsList.start;

	if (debug) {
		while (control != nullptr)
		{
			if (control->data->toDraw) {
				control->data->Draw(app->render);
				control->data->DrawDebug(app->render);
			}
			control = control->next;
		}
	}
	else {
		while (control != nullptr)
		{
			if (control->data->toDraw) {
				control->data->Draw(app->render);
			}
			control = control->next;
		}
	}

	return true;

}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}

void GuiManager::Clear(GuiControl* toClear)
{

	ListItem<GuiControl*>* item;

	for (item = guiControlsList.start; item != NULL; item = item->next)
	{
		if (toClear == item->data)
		{
			RELEASE(item->data);
			guiControlsList.Del(item);
			return;
		}
	}
}



