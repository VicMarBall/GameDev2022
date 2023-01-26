#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, const char* text, int sliderLength);
	virtual ~GuiSlider();

	bool Update(float dt);
	bool Draw(Render* render);
	bool DrawDebug(Render* render);

	void TurnON() {
		toDraw = true;
		canClick = true;
	}

	void TurnOFF() {
		toDraw = false;
		canClick = false;
	}

	void SetValue(int value) {
		this->valueSlider = value;
		if (valueSlider < 0) {
			valueSlider = 0;
		}
		else if (valueSlider > sliderLength) {
			valueSlider = sliderLength;
		}
		box.x = bounds.x + valueSlider - (box.w * 0.5f);
	}

	int sliderLength;

	int valueSlider;

	int mouseX, mouseY;
	int mousePreviousX, mousePreviousY;

	bool canClick = true;
	bool drawBasic = false;

	SDL_Rect box;

	uint audioFxId;
};

#endif // __GUISLIDER_H__