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

	void EndCollision(PhysBody* otherBody);

	void SetPosition(int posX, int posY);

	void SetDefaultObjective();

	void Die();

	void Move();

	void PauseMovement();

	void ResumeMovement();

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

	PhysBody* ground = nullptr;

};

#endif // __WALKINGENEMY_H__