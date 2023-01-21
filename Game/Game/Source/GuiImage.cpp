#include "GuiImage.h"
#include "Render.h"
#include "App.h"
#include "Log.h"

GuiImage::GuiImage(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::IMAGE, id)
{
	this->bounds = bounds;

	toDraw = true;
}

GuiImage::~GuiImage()
{

}

bool GuiImage::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	if (texture != nullptr) {
		if (toDraw) {
			app->render->DrawTexture(texture, bounds.x, bounds.y, 0, false);
		}
	}

	return false;
}