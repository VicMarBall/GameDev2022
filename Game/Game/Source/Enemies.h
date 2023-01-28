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
		OPTICK_EVENT();
		return true;
	}

	virtual bool CleanUp(){
		return true;
	}

	virtual void OnCollision(PhysBody* otherBody) {

	};

	virtual void EndCollision(PhysBody* otherBody) {

	};


public:

	virtual void SetPosition(int posX, int posY) {

	};

	virtual void Die() {

	};



	virtual void SetObjective(iPoint pos)
	{
		objective = pos;
	}

	iPoint GetObjective()
	{
		return objective;
	}

	void SetPath(const DynArray<iPoint>* p)
	{
		path.Clear();
		for (int i = 0; i < p->Count(); ++i) {
			path.PushBack(*p->At(i));
		}
	}

	const DynArray<iPoint>* GetPath()
	{
		return &path;
	}

	int GetRadiusPath() {
		return radiusPath;
	}

	bool IsInRadius(iPoint p) {
		return position.DistanceTo(p) < radiusPath;
	}

	virtual void SetDefaultObjective()
	{

	}

	virtual void PauseMovement() {}

	virtual void ResumeMovement() {}

protected:

	int radiusPath;

	DynArray<iPoint> path;

	iPoint objective;

	Directions facing = RIGHT;

};

#endif //__ENEMIES_H__