#ifndef __FLYINGENEMY_H__
#define __FLYINGENEMY_H__

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

	void SetPosition(int posX, int posY);

	void Die();

private:

	bool isAlive;

	SDL_Texture* texture;
	const char* texturePath;

	Animation flyingRight;
	Animation flyingLeft;
	Animation death;

	Animation* previousAnimation;

	//TODO 4: Add a physics to an item
	PhysBody* pBody;

};

#endif // __FLYINGENEMY_H__