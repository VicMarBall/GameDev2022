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
	//L02: DONE 5: Get Player parameters from XML


	return true;
}

bool Bullet::Start() {

	// texturePath = parameters.attribute("texturepath").as_string();

	texturePath = "Assets/Textures/bullet.png";
	sfx = app->audio->LoadFx("Assets/Audio/Fx/shoot.wav");

	//initilize textures
	texture = app->tex->Load(texturePath);

	rightAnimation.PushBack({ 0, 0, 32, 32 });
	rightAnimation.PushBack({ 32, 0, 32, 32 });
	rightAnimation.speed = 0.5f;
	rightAnimation.loop = false;

	leftAnimation.PushBack({ 0, 32, 32, 32 });
	leftAnimation.PushBack({ 32, 32, 32, 32 });
	leftAnimation.speed = 0.5f;
	leftAnimation.loop = false;

	// L07 TODO 5: Add physics to the player - initialize physics body
	pBody = app->physics->CreateRectangleSensor(position.x, position.y, 32, 32, DYNAMIC);
	pBody->body->SetBullet(true);
	pBody->body->SetFixedRotation(true);
	pBody->body->SetActive(true);
	pBody->body->SetGravityScale(0);
	pBody->listener = app->entityManager;
	pBody->entity = this;

	timer = 0;

	isAvailable = true;

	return true;
}

bool Bullet::Update()
{
	if (toDisable) {
		isAvailable = true;
		pBody->body->SetActive(false);
	}

	Animation* currentAnimation;
	if (velocity < 0) {
		currentAnimation = &leftAnimation;
	}
	else {
		currentAnimation = &rightAnimation;
	}

	SDL_Rect currentFrame = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	// L07 TODO 5: Add physics to the player - updated player position using physics
	if (pBody != nullptr && !isAvailable) {
		if (timer < 10) {
			pBody->body->SetLinearVelocity({ velocity, 0 });
			pBody->GetPosition(position.x, position.y);
			app->render->DrawTexture(texture, position.x + 16, position.y + 16, &currentFrame);
		}
		else {
			DisableBullet();
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
		app->physics->DeleteBody(pBody);
	}


	active = false;

	return true;
}

void Bullet::OnCollision(PhysBody* otherBody)
{
	if (otherBody->typeTerrain == WALL) {
		DisableBullet();
	}

	if (otherBody->entity != nullptr) {
		if (otherBody->entity->type == EntityType::WALKINGENEMY) {
			DisableBullet();
		}
		if (otherBody->entity->type == EntityType::FLYINGENEMY) {
			DisableBullet();
		}
	}

}

void Bullet::SetBullet(DIRECTIONS direction, iPoint pos)
{
	toDisable = false;

	timer = 0;

	pBody->body->SetActive(true);

	isAvailable = false;

	app->audio->PlayFx(sfx);

	if (pBody != nullptr) {
		b2Vec2 position = { PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y) };
		pBody->body->SetTransform(position, 0);

		leftAnimation.Reset();
		rightAnimation.Reset();

		switch (direction)
		{
		case Bullet::LEFT:
			velocity = -20;
			break;
		case Bullet::RIGHT:
			velocity = 20;
			break;
		default:
			break;
		}

	}
	
}

void Bullet::DisableBullet()
{
	toDisable = true;
}

bool Bullet::IsAvailable()
{
	return isAvailable;
}
