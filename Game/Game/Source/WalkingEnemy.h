#ifndef __WALKINGENEMY_H__
#define __WALKINGENEMY_H__

#include "Enemies.h"
#include "Point.h"
#include "DynArray.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class WalkingEnemy : public Enemy
{
public:

	WalkingEnemy();
	virtual ~WalkingEnemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

public:

	void OnCollision(PhysBody* otherBody);

	void SetPosition(int posX, int posY);

	void Die();

	void SetObjective(iPoint pos);

	iPoint GetObjective();

	void SetPath(const DynArray<iPoint>* p);

	const DynArray<iPoint>* GetPath();

private:

	SDL_Texture* texture;
	const char* texturePath;
	bool isAlive;

	Animation walkingRight;
	Animation walkingLeft;
	Animation death;

	Animation* previousAnimation;

	//TODO 4: Add a physics to an item
	PhysBody* pBody;

	const DynArray<iPoint>* path;

	iPoint objective;

};

#endif // __WALKINGENEMY_H__