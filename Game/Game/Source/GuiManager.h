#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 bool Update(float dt);

	bool Draw();

	// Called before quitting
	bool CleanUp();

	void Clear(GuiControl* toClear);

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, int sliderLength = 100);

public:

	List<GuiControl*> guiControlsList;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	bool debug;

	SDL_Texture* texture;

};

#endif // __GUIMANAGER_H__
