#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckbox : public GuiControl
{
public:

	GuiCheckbox(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiCheckbox();

	bool Update(float dt);
	bool Draw(Render* render);

	void SwitchCheck();

	void SetCheck(bool state);

	void TurnON() {
		toDraw = true;
		canClick = true;
	}

	void TurnOFF() {
		toDraw = false;
		canClick = false;
	}

	int mouseX, mouseY;

	bool checked;

	bool canClick = true;
	bool drawBasic = false;

	SDL_Rect box;

	uint audioFxId;
};

#endif // __GUIBUTTON_H__