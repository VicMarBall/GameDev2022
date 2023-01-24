#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw(Render* render);

	virtual void TurnON() {
		toDraw = true;
		canClick = true;
	}

	virtual void TurnOFF() {
		toDraw = false;
		canClick = false;
	}

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	uint audioFxId;
};

#endif // __GUIBUTTON_H__