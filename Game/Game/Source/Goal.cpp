#include "Goal.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "LevelOne.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Goal::Goal() : Entity(EntityType::GOAL)
{
	name.Create("Goal");
}

Goal::~Goal() {

}

bool Goal::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML


	return true;
}

bool Goal::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 TODO 5: Add physics to the player - initialize physics body
	pBody = app->physics->CreateRectangle(position.x, position.y, 32, 32, STATIC);
	pBody->body->SetFixedRotation(true);
	pBody->body->SetActive(true);
	pBody->listener = app->entityManager;
	pBody->entity = this;

	win = false;

	return true;
}

bool Goal::Update()
{
	// L07 TODO 5: Add physics to the player - updated player position using physics

	if (!win) {
		pBody->GetPosition(position.x, position.y);
		app->render->DrawTexture(texture, position.x + 1, position.y + 1);
	}
	else {
		pBody->body->SetActive(false);
	}


	return true;
}

bool Goal::CleanUp()
{
	app->tex->UnLoad(texture);
	texture = nullptr;
	texturePath = nullptr;
	if (pBody != nullptr) {
		app->physics->world->DestroyBody(pBody->body);
	}
	

	active = false;

	return true;
}

void Goal::OnCollision(PhysBody* otherBody)
{
	if (otherBody->entity != nullptr) {
		if (otherBody->entity->type == EntityType::PLAYER) {
			win = true;
			LOG("WIN");
		}
	}
}
