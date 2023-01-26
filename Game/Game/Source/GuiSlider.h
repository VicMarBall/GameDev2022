#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

#define MIN_VALUE 0
#define MAX_VALUE 100

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, const char* text, int sliderLength);
	virtual ~GuiSlider();

	bool Update(float dt);
	bool Draw(Render* render);

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
		if (valueSlider < MIN_VALUE) {
			valueSlider = MIN_VALUE;
		}
		else if (valueSlider > MAX_VALUE) {
			valueSlider = MAX_VALUE;
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