#ifndef __LEVELONE_H__
#define __LEVELONE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"

struct SDL_Texture;

class LevelOne : public Module
{
public:

	LevelOne();

	// Destructor
	virtual ~LevelOne();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

private:
	SDL_Texture* img;

};

#endif // __LEVELONE_H__