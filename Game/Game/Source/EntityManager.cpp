#include "EntityManager.h"
#include "Bullet.h"
#include "Player.h"
#include "Goal.h"
#include "Coin.h"
#include "Enemies.h"
#include "WalkingEnemy.h"
#include "FlyingEnemy.h"
#include "ExtraLife.h"
#include "Checkpoint.h"
#include "App.h"
#include "Textures.h"
#include "LevelOne.h"
#include "LevelTwo.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	//L02: DONE 2: Instantiate entity according to the type and add the new entoty it to the list of Entities

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::GOAL:
		entity = new Goal;
		break;
	case EntityType::COIN:
		entity = new Coin();
		break;
	case EntityType::WALKINGENEMY:
		entity = new WalkingEnemy();
		break;
	case EntityType::FLYINGENEMY:
		entity = new FlyingEnemy();
		break;
	case EntityType::BULLET:
		entity = new Bullet();
		break;
	case EntityType::EXTRALIFE:
		entity = new ExtraLife();
		break;
	case EntityType::CHECKPOINT:
		entity = new Checkpoint();
		break;
	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) {
			entity->pendingToDelete = true;
		}
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

void EntityManager::OnCollision(PhysBody* pBody1, PhysBody* pBody2)
{
	if (pBody1->entity != nullptr) {
		pBody1->entity->OnCollision(pBody2);
	}
}

void EntityManager::EndCollision(PhysBody* pBody1, PhysBody* pBody2)
{
	if (pBody1->entity != nullptr) {
		pBody1->entity->EndCollision(pBody2);
	}
}

void EntityManager::DestroyAllActiveEntities()
{
	for (int i = entities.Count() - 1; i >= 0; --i) {
		Entity* entity = entities.At(i)->data;
		if (entity !=  nullptr) {
			if (entity->active) {
				DestroyEntity(entity);
			}
		}
	}
	// entities.Clear();
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update();

		if (pEntity->pendingToDelete == true) {
			item->data->CleanUp();
			entities.Del(item);
		}
	}

	return ret;
}