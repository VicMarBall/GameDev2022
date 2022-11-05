#ifndef __GOAL_H__
#define __GOAL_H__

#include "Entity.h"
#include "Point.h" 
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Goal : public Entity
{
public:

	Goal();

	virtual ~Goal();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* otherBody);

public:

private:

	bool win;
	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pBody;

};

#endif // __PLAYER_H__