#ifndef __LEVELONE_H__
#define __LEVELONE_H__

#include "Module.h"
#include "Player.h"
#include "Goal.h"
#include "Coin.h"
#include "Enemies.h"
#include "List.h"

#include "GuiImage.h"

#define MAX_ENEMIES 20

struct SDL_Texture;

class LevelOne : public Module
{
public:

	LevelOne(bool CurrentState);

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
	pugi::xml_node playerParameters;
	pugi::xml_node goalParameters;
	pugi::xml_node enemyParameters[MAX_ENEMIES];

	Player* player;
	Goal* goal;
	Enemy* enemy[MAX_ENEMIES];

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

	int enemyCount = 0;

	SDL_Texture* lifeTexture;

	GuiImage* livesUI[3];
};

#endif // __LEVELONE_H__