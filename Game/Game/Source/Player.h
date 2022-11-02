#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* otherBody);

	void EndCollision(PhysBody* otherBody);

public:

private:

	bool onAir;
	bool canDoubleJump;

	bool groundPounding;

	float maxSpeed;

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	Animation idle;
	Animation walking;
	Animation jumping;

	// L07 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pBody;

};

#endif // __PLAYER_H__