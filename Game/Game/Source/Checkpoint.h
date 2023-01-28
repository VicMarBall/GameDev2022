#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Checkpoint : public Entity
{
public:

	Checkpoint();
	virtual ~Checkpoint();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* otherBody);

	bool CheckPicking();

public:

	bool isPicked = false;
	int id;

private:

	SDL_Texture* texture;
	const char* texturePath;

	//TODO 4: Add a physics to an item
	PhysBody* pBody;

};

#endif // __CHECKPOINT_H__