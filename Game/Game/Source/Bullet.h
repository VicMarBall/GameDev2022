#ifndef __BULLET_H__
#define __BULLET_H__

#include "Entity.h"
#include "Point.h" 
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Bullet : public Entity
{
public:

	Bullet();

	virtual ~Bullet();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* otherBody);

public:

	enum DIRECTIONS {
		LEFT,
		RIGHT
	};

	void SetBullet(DIRECTIONS direction, iPoint pos);

	void DisableBullet();

	bool IsAvailable();

	void PauseMovement();

	void ResumeMovement();

private:

	float timer;
	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	int sfx;

	float velocity;

	bool isAvailable;

	bool toDisable;

	// L07 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pBody;

	Animation leftAnimation;
	Animation rightAnimation;
};

#endif // __BULLET_H__