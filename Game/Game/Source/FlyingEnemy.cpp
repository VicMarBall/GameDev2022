#include "FlyingEnemy.h"
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

	defaultObjective = position;

	//initilize textures
	texture = app->tex->Load(texturePath);

	flyingRight.PushBack({ 32, 0, 32, 32 });
	flyingRight.PushBack({ 0, 0, 32, 32 });
	flyingRight.speed = 0.1f;

	flyingLeft.PushBack({ 32, 32, 32, 32 });
	flyingLeft.PushBack({ 0, 32, 32, 32 });
	flyingLeft.speed = 0.1f;

	death.PushBack({ 32, 64, 32, 32 });
	death.PushBack({ 0, 64, 32, 32 });
	death.speed = 0.1f;
	death.loop = false;

	// L07 TODO 5: Add physics to the player - initialize physics body
	pBody = app->physics->CreateRectangle(position.x, position.y, 16, 16, DYNAMIC);
	pBody->body->SetFixedRotation(true);
	pBody->listener = app->entityManager;
	pBody->entity = this;
	pBody->body->SetGravityScale(0);

	isAlive = true;

	pBody->body->SetActive(true);

	previousAnimation = &flyingLeft;

	radiusPath = 200;

	return true;
}

bool FlyingEnemy::Update() {

	Animation* currentAnimation = previousAnimation;
	SDL_Rect currentFrame = currentAnimation->GetCurrentFrame();

	if (isAlive) {
		Move();
	}
	else {
		pBody->body->SetActive(false);
		currentAnimation = &death;
	}

	if (facing == LEFT) {
		currentAnimation = &flyingLeft;
	}
	else if (facing == RIGHT) {
		currentAnimation = &flyingRight;
	}

	currentFrame = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	pBody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x -8, position.y -8, &currentFrame);

	if (death.HasFinished()) {
		pBody->body->SetActive(false);
		active = false;
		app->entityManager->DestroyEntity(this);
	}

	previousAnimation = currentAnimation;

	return true;
}

bool FlyingEnemy::CleanUp() {
	app->tex->UnLoad(texture);
	texture = nullptr;
	texturePath = nullptr;

	flyingRight.FullReset();
	flyingLeft.FullReset();
	death.FullReset();

	previousAnimation = nullptr;

	if (pBody != nullptr) {
		app->physics->DeleteBody(pBody);
	}

	active = false;

	return true;
}

void FlyingEnemy::OnCollision(PhysBody* otherBody) {
	if (otherBody->entity != nullptr) {
		if (otherBody->entity->type == EntityType::BULLET) {
			Die();
		}
	}
}

void FlyingEnemy::SetPosition(int posX, int posY) {
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pBody->body->SetTransform(position, 0);
}

void FlyingEnemy::Die() {
	isAlive = false;
	facing = NO;
}

void FlyingEnemy::SetObjective(iPoint pos)
{
	if (defaultObjective.DistanceTo(pos) < radiusPath) {
		objective = pos;
	}
	else {
		objective = defaultObjective;
	}
}

void FlyingEnemy::SetDefaultObjective()
{
	SetObjective(defaultObjective);
}


void FlyingEnemy::Move()
{
	b2Vec2 velocity = pBody->body->GetLinearVelocity();

	facing = NO;
	elevate = NO;

	iPoint* tile0 = path.At(0);
	iPoint* tile1 = path.At(1);
	iPoint* tile2 = path.At(2);


	if (tile0 != nullptr) {
		if (tile1 != nullptr) {
			if (tile2 != nullptr) {
				if (tile2->x < tile1->x) {
					facing = LEFT;
				}
				else if (tile2->x > tile1->x) {
					facing = RIGHT;
				}

				if (tile2->y < tile1->y) {
					elevate = UP;
				}
				else if (tile2->y > tile1->y) {
					elevate = DOWN;
				}
			}
			if (tile1->x < tile0->x) {
				facing = LEFT;
			}
			else if (tile1->x > tile0->x) {
				facing = RIGHT;
			}

			if (tile1->y < tile0->y) {
				elevate = UP;
			}
			else if (tile1->y > tile0->y) {
				elevate = DOWN;
			}
		}
	}

	if (position.x + 8 < tile0->x + 16) {

	}




	//if (IsInRadius(objective)) {
	//	if (objective.x > position.x) {
	//		facing = RIGHT;
	//	}
	//	if (objective.x < position.x) {
	//		facing = LEFT;
	//	}
	//	if (objective.y < position.y) {
	//		elevate = UP;
	//	}
	//	if (objective.y > position.y) {
	//		elevate = DOWN;
	//	}
	//}
	//else {
	//	facing = NO;
	//	elevate = NO;
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
		velocity.x = 0;
		break;
	default:
		break;
	}

	switch (elevate)
	{
	case Entity::UP:
		velocity.y = -2;
		break;
	case Entity::DOWN:
		velocity.y = 2;
		break;
	case Entity::NO:
		velocity.y = 0;
		break;
	default:
		break;
	}

	pBody->body->SetLinearVelocity(velocity);
}