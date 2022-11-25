#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "Entity.h"
#include "Point.h" 
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();

	virtual ~Enemy();

	virtual bool Awake();

	virtual bool Start();

	virtual bool Update();

	virtual bool CleanUp();

	virtual void OnCollision(PhysBody* otherBody);

public:

private:


};

#endif //__ENEMIES_H__