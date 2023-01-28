#ifndef __CARROT_H__
#define __CARROT_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Carrot : public Entity
{
public:

	Carrot();
	virtual ~Carrot();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* otherBody);

	bool CheckPickingCarrot();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;

	int sfx;

	//TODO 4: Add a physics to an item
	PhysBody* pBody;

};

#endif // __CARROT_H__