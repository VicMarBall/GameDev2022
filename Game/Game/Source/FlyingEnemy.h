#ifndef __FLYINGENEMY_H__
#define __FLYINGENEMY_H__

#include "Entity.h"
#include "Enemies.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class FlyingEnemy : public Enemy
{
public:

	FlyingEnemy();
	virtual ~FlyingEnemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

public:

	void OnCollision(PhysBody* otherBody);

	void SetPosition(int posX, int posY);

	void Die();

	void SetObjective(iPoint pos);

	void SetDefaultObjective();

	void Move();

private:

	bool isAlive;

	iPoint defaultObjective;

	SDL_Texture* texture;
	const char* texturePath;

	Animation flyingRight;
	Animation flyingLeft;
	Animation death;

	Animation* previousAnimation;

	//TODO 4: Add a physics to an item
	PhysBody* pBody;

	Directions elevate = Directions::NO;

};

#endif // __FLYINGENEMY_H__