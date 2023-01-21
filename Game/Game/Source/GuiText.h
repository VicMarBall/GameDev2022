#ifndef __GUITEXT_H__
#define __GUITEXT_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiText : public GuiControl
{
public:

	GuiText(uint32 id, const char* text, SDL_Rect bounds);
	virtual ~GuiText();

	bool Draw(Render* render);

	void SetColor(SDL_Color color);

	void SetText(const char* text);

	SDL_Color colorText;
};

#endif // __GUIIMAGE_H__