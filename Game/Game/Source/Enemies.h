#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "Entity.h"
#include "Point.h" 
#include "DynArray.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;


class Enemy : public Entity
{
public:

	Enemy(EntityType enemyType) :Entity(enemyType) {

	}

	virtual ~Enemy() {

	};

	virtual bool Awake(){
		return true;
	}

	virtual bool Start(){
		return true;
	}

	virtual bool Update(){
		return true;
	}

	virtual bool CleanUp(){
		return true;
	}

	virtual void OnCollision(PhysBody* otherBody) {

	};

public:

	virtual void SetPosition(int posX, int posY) {

	};

	virtual void Die() {

	};



	void SetObjective(iPoint pos)
	{
		objective = pos;
	}

	iPoint GetObjective()
	{
		return objective;
	}

	void SetPath(const DynArray<iPoint>* p)
	{
		path = p;
	}

	const DynArray<iPoint>* GetPath()
	{
		return path;
	}

	int GetRadiusPath() {
		return radiusPath;
	}


protected:

	int radiusPath;

private:

	const DynArray<iPoint>* path;

	iPoint objective;

};

#endif //__ENEMIES_H__