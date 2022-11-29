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

	flyingRight.PushBack({ 32, 0, 32, 32 });
	flyingRight.PushBack({ 0, 0, 32, 32 });
	flyingRight.speed = 0.2f;

	flyingLeft.PushBack({ 32, 32, 32, 32 });
	flyingLeft.PushBack({ 0, 32, 32, 32 });
	flyingLeft.speed = 0.2f;

	death.PushBack({ 32, 64, 32, 32 });
	death.PushBack({ 0, 64, 32, 32 });

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

	b2Vec2 velocity = pBody->body->GetLinearVelocity();
	Animation* currentAnimation = previousAnimation;
	SDL_Rect currentFrame = currentAnimation->GetCurrentFrame();

	pBody->body->SetLinearVelocity(velocity);

	currentFrame = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	previousAnimation = currentAnimation;

	pBody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x + 1, position.y + 1, &currentFrame);

	return true;
}

bool FlyingEnemy::CleanUp() {
	app->tex->UnLoad(texture);
	texture = nullptr;
	texturePath = nullptr;

	flyingRight.FullReset();
	flyingLeft.FullReset();

	death.FullReset();

	if (pBody != nullptr) {
		app->physics->world->DestroyBody(pBody->body);
	}



	active = false;

	return true;
}

void FlyingEnemy::SetPosition(int posX, int posY) {

}

void FlyingEnemy::Die() {

}