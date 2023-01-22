#ifndef __EXTRALIFE_H__
#define __EXTRALIFE_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class ExtraLife : public Entity
{
public:

	ExtraLife();
	virtual ~ExtraLife();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* otherBody);

	bool CheckPickingLife();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;

	//TODO 4: Add a physics to an item
	PhysBody* pBody;

};

#endif // __EXTRALIFE_H__