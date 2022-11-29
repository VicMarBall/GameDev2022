#include "FlyingEnemy.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "LevelOne.h"
#include "LevelTwo.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

FlyingEnemy::FlyingEnemy() : Enemy(EntityType::FLYINGENEMY) {

	name.Create("flying_enemy");

}


FlyingEnemy::~FlyingEnemy() {

}

bool FlyingEnemy::Awake() {

	return true;
}

bool FlyingEnemy::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	//initilize textures
	texture = app->tex->Load(texturePath);

	flyingRight.PushBack({ 0, 32, 32, 32 });
	flyingRight.PushBack({ 0, 0, 32, 32 });
	flyingRight.speed = 0.2f;

	flyingLeft.PushBack({ 96, 32, 32, 32 });
	flyingLeft.PushBack({ 96, 0, 32, 32 });
	flyingLeft.speed = 0.2f;

	death.PushBack({ 32, 32, 32, 32 });

	// L07 TODO 5: Add physics to the player - initialize physics body
	pBody = app->physics->CreateRectangle(position.x, position.y, 32, 32, DYNAMIC);
	pBody->body->SetFixedRotation(true);
	pBody->listener = app->entityManager;
	pBody->entity = this;

	isAlive = true;

	previousAnimation = &flyingLeft;

	return true;
}

bool FlyingEnemy::Update() {
	return true;
}

bool FlyingEnemy::CleanUp() {
	return true;
}

void FlyingEnemy::SetPosition(int posX, int posY) {

}

void FlyingEnemy::Die() {

}