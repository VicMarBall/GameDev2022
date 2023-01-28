#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "App.h"

enum class EntityType
{
	PLAYER,
	CARROT,
	GOAL,
	WALKINGENEMY,
	FLYINGENEMY,
	BULLET, 
	EXTRALIFE,
	CHECKPOINT,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update()
	{
		OPTICK_EVENT();
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* otherBody) {
	
	};

	virtual void EndCollision(PhysBody* otherBody) {

	};

	enum Directions {
		LEFT,
		RIGHT,
		UP,
		DOWN,
		NO
	};

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters;
	bool pendingToDelete = false;
	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;
};

#endif // __ENTITY_H__