#include "GuiText.h"
#include "Render.h"
#include "App.h"
#include "Log.h"

GuiText::GuiText(uint32 id, const char* text, SDL_Rect bounds) : GuiControl(GuiControlType::TEXT, id)
{
	this->bounds = bounds;

	this->text = text;

	colorText = { 0, 0, 0, 255 };
}

GuiText::~GuiText()
{

}

bool GuiText::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, colorText);

	return false;
}

void GuiText::SetColor(SDL_Color color)
{
	colorText = color;
}

void GuiText::SetText(const char* text)
{
	this->text = text;
}
