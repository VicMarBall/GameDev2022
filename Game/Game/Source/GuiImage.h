#ifndef __GUIIMAGE_H__
#define __GUIIMAGE_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiImage : public GuiControl
{
public:

	GuiImage(uint32 id, SDL_Rect bounds);
	virtual ~GuiImage();

	bool Draw(Render* render);
};

#endif // __GUIIMAGE_H__