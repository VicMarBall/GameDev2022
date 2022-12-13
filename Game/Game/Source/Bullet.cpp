#include "Bullet.h"
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

Bullet::Bullet() : Entity(EntityType::BULLET)
{
	name.Create("Bullet");
	Start();
}

Bullet::~Bullet() {

}

bool Bullet::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML


	return true;
}

bool Bullet::Start() {

	// texturePath = parameters.attribute("texturepath").as_string();

	texturePath = "Assets/Textures/gun.png";

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 TODO 5: Add physics to the player - initialize physics body
	pBody = app->physics->CreateRectangleSensor(position.x, position.y, 32, 32, DYNAMIC);
	pBody->body->SetFixedRotation(true);
	pBody->body->SetActive(true);
	pBody->body->SetGravityScale(0);
	pBody->listener = app->entityManager;
	pBody->entity = this;

	timer = 0;

	return true;
}

bool Bullet::Update()
{
	// L07 TODO 5: Add physics to the player - updated player position using physics
	if (pBody != nullptr) {
		if (timer < 10) {
			pBody->body->SetLinearVelocity({ velocity, 0 });
			pBody->GetPosition(position.x, position.y);
			app->render->DrawTexture(texture, position.x + 1, position.y + 1);
		}
		else {
			app->entityManager->DestroyEntity(this);
		}
	}

	timer += 0.1f;


	return true;
}

bool Bullet::CleanUp()
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

void Bullet::OnCollision(PhysBody* otherBody)
{
	if (otherBody->typeTerrain == WALL) {
		app->entityManager->DestroyEntity(this);
	}

	if (otherBody->entity != nullptr) {
		if (otherBody->entity->type == EntityType::WALKINGENEMY) {
			app->entityManager->DestroyEntity(this);
		}
		if (otherBody->entity->type == EntityType::FLYINGENEMY) {
			app->entityManager->DestroyEntity(this);
		}
	}

}

void Bullet::SetBullet(DIRECTIONS direction, iPoint pos)
{
	if (pBody != nullptr) {
		b2Vec2 position = { PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y) };
		pBody->body->SetTransform(position, 0);

		switch (direction)
		{
		case Bullet::LEFT:
			velocity = -25;
			break;
		case Bullet::RIGHT:
			velocity = 25;
			break;
		default:
			break;
		}

	}
	
}
