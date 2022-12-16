#ifndef __LEVELTWO_H__
#define __LEVELTWO_H__

#include "Module.h"
#include "Player.h"
#include "Goal.h"
#include "Item.h"
#include "Enemies.h"

struct SDL_Texture;

class LevelTwo : public Module
{
public:

	LevelTwo(bool CurrentState);

	// Destructor
	virtual ~LevelTwo();

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
	Goal* goal;
	Enemy* enemy;
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

private:
	SDL_Texture* img;
	char* musicPath;
	SString mapFileName;

	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	int camX;
	int camY;

	DynArray<iPoint> debugPath;

};

#endif // __LEVELONE_H__