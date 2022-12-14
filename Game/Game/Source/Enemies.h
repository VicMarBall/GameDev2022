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

	virtual void SetObjective(iPoint pos) {

	}

	virtual iPoint GetObjective() {

		iPoint ret;
		ret.x = 0;
		ret.y = 0;

		return ret;
	}

	virtual void SetPath(const DynArray<iPoint>* p) {

	}

	virtual const DynArray<iPoint>* GetPath() {
		DynArray<iPoint>* ret;

		return ret;
	}


private:


};

#endif //__ENEMIES_H__