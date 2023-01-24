#include "WalkingEnemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "PathFinding.h"
#include "Render.h"
#include "LevelOne.h"
#include "LevelTwo.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"


WalkingEnemy::WalkingEnemy() : Enemy(EntityType::WALKINGENEMY) {

	name.Create("walking_enemy");

}


WalkingEnemy::~WalkingEnemy() {

}

bool WalkingEnemy::Awake() {

	return true;
}

bool WalkingEnemy::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	//initilize textures
	texture = app->tex->Load(texturePath);

	walkingRight.PushBack({ 32, 0, 32, 32 });
	walkingRight.PushBack({ 0, 0, 32, 32 });
	walkingRight.speed = 0.1f;

	walkingLeft.PushBack({ 32, 32, 32, 32 });
	walkingLeft.PushBack({ 0, 32, 32, 32 });
	walkingLeft.speed = 0.1f;

	death.PushBack({ 32, 64, 32, 32 });
	death.PushBack({ 0, 64, 32, 32 });
	death.speed = 0.1f;
	death.loop = false;

	// L07 TODO 5: Add physics to the player - initialize physics body
	pBody = app->physics->CreateRectangle(position.x, position.y, 16, 16, DYNAMIC);
	pBody->body->SetFixedRotation(true);
	pBody->listener = app->entityManager;
	pBody->entity = this;

	isAlive = true;

	previousAnimation = &walkingLeft;

	radiusPath = 120;

	return true;
}

bool WalkingEnemy::Update() {

	Animation* currentAnimation = previousAnimation;
	SDL_Rect currentFrame = currentAnimation->GetCurrentFrame();

	if (isAlive) {
		Move();
	}
	else {
		currentAnimation = &death;
	}

	if (facing == LEFT) {
		currentAnimation = &walkingLeft;
	}
	else if (facing == RIGHT) {
		currentAnimation = &walkingRight;
	}

	currentFrame = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	pBody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x - 8, position.y - 15, &currentFrame);

	if (death.HasFinished()) {
		pBody->body->SetActive(false);
		active = false;
		app->entityManager->DestroyEntity(this);
	}

	previousAnimation = currentAnimation;

	return true;
}

bool WalkingEnemy::CleanUp() {
	ground = nullptr;

	app->tex->UnLoad(texture);
	texture = nullptr;
	texturePath = nullptr;

	walkingRight.FullReset();
	walkingLeft.FullReset();
	death.FullReset();

	previousAnimation = nullptr;

	if (pBody != nullptr) {
		app->physics->DeleteBody(pBody);
	}

	active = false;

	return true;
}

void WalkingEnemy::OnCollision(PhysBody* otherBody) {
	if (otherBody->typeTerrain == TypeTerrain::FLOOR || otherBody->typeTerrain == TypeTerrain::FLOATING) {
		ground = otherBody;
	}
	if (otherBody->typeTerrain == TypeTerrain::DEATH) {
		Die();
	}
	if (otherBody->entity != nullptr) {
		if (otherBody->entity->type == EntityType::BULLET) {
			Die();
		}
	}
}

void WalkingEnemy::EndCollision(PhysBody* otherBody)
{
	if (otherBody->typeTerrain == TypeTerrain::FLOOR || otherBody->typeTerrain == TypeTerrain::FLOATING) {
		ground = nullptr;
	}
}

void WalkingEnemy::SetPosition(int posX, int posY) {
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pBody->body->SetTransform(position, 0);
}

void WalkingEnemy::SetDefaultObjective()
{
	if (ground != nullptr) {
		int posGroundX;
		int posGroundY;
		ground->GetPosition(posGroundX, posGroundY);
		if (facing == LEFT) {
			iPoint obj = { posGroundX + 16 , position.y };
			SetObjective(obj);
		}
		if (facing == RIGHT) {
			iPoint obj = { posGroundX + (2 * ground->width) - 32 , position.y };
			SetObjective(obj);
		}

	}
	else {
		SetObjective({ position.x + 8, position.y + 8 });
	}
}

void WalkingEnemy::Die() {
	isAlive = false;
	facing = NO;
}

void WalkingEnemy::Move()
{
	b2Vec2 velocity = pBody->body->GetLinearVelocity();

	if (objective.x > position.x) {
		facing = RIGHT;
	}
	if (objective.x < position.x) {
		facing = LEFT;
	}
	//else {
	//	if (ground != nullptr) {
	//		int posGroundX;
	//		int posGroundY;
	//		ground->GetPosition(posGroundX, posGroundY);
	//		if (position.x + 8 < posGroundX + 16) {
	//			facing = RIGHT;
	//		}
	//		else if (position.x + 8 > posGroundX + (2*ground->width) - 32) {
	//			facing = LEFT;
	//		}
	//	}
	//}

	switch (facing)
	{
	case Entity::LEFT:
		velocity.x = -2;
		break;
	case Entity::RIGHT:
		velocity.x = 2;
		break;
	case Entity::NO:
		break;
	default:
		break;
	}

	pBody->body->SetLinearVelocity(velocity);
}

void WalkingEnemy::PauseMovement()
{
	pBody->body->SetActive(false);
}

void WalkingEnemy::ResumeMovement()
{
	pBody->body->SetActive(true);
}