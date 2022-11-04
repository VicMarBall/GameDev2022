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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	

	return true;
}

bool Player::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	//initilize textures
	texture = app->tex->Load(texturePath);

	idleRight.PushBack({0, 0, 32, 32});
	idleRight.PushBack({ 32, 0, 32, 32 });
	idleRight.speed = 0.1f;

	idleLeft.PushBack({ 96, 0, 32, 32 });
	idleLeft.PushBack({ 64, 0, 32, 32 });
	idleLeft.speed = 0.1f;

	walkingRight.PushBack({ 0, 32, 32, 32 });
	walkingRight.PushBack({0, 0, 32, 32});
	walkingRight.speed = 0.2f;

	walkingLeft.PushBack({ 96, 32, 32, 32 });
	walkingLeft.PushBack({ 96, 0, 32, 32 });
	walkingLeft.speed = 0.2f;

	jumpingRight.PushBack({0, 32, 32, 32});

	jumpingLeft.PushBack({ 96, 32, 32, 32 });

	death.PushBack({32, 32, 32, 32});

	// L07 TODO 5: Add physics to the player - initialize physics body
	pBody = app->physics->CreateRectangle(position.x, position.y, 32, 32, DYNAMIC);
	pBody->body->SetFixedRotation(true);
	pBody->listener = app->entityManager;
	pBody->entity = this;

	isAlive = true;

	previousAnimation = &idleRight;

	onAir = true;

	maxSpeed = 6;

	return true;
}

bool Player::Update()
{
	// L07 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = pBody->body->GetLinearVelocity();
	Animation* currentAnimation = previousAnimation;
	SDL_Rect currentFrame = currentAnimation->GetCurrentFrame();

	if (isAlive) {
		if (previousAnimation == &idleRight ||
			previousAnimation == &walkingRight ||
			previousAnimation == &jumpingRight) {
			currentAnimation = &idleRight;
		}
		else if (previousAnimation == &idleLeft ||
			previousAnimation == &walkingLeft ||
			previousAnimation == &jumpingLeft) {
			currentAnimation = &idleLeft;
		}

		if (!godMode) {
			//L02: DONE 4: modify the position of the player using arrow keys and render the texture
		// jump
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				if (onAir) {
					if (canDoubleJump || godMode) {
						velocity.y = -10;
						canDoubleJump = false;
					}
				}
				else {
					velocity.y = -10;
				}
			}

			// groundPound
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
				if (onAir) {
					groundPounding = true;
					velocity.y = 20;
					velocity.x = 0;
				}
			}

			if (velocity.x < 0) {
				currentAnimation = &walkingLeft;
				walkingLeft.speed = abs(velocity.x) * 0.03f;
			}

			if (velocity.x > 0) {
				currentAnimation = &walkingRight;
				walkingRight.speed = abs(velocity.x) * 0.03f;
			}

			// go left
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				if (!onAir) {
					pBody->body->ApplyForceToCenter({ -60,0 }, true);
				}
				else {
					pBody->body->ApplyForceToCenter({ -30,0 }, true);
				}
				currentAnimation = &walkingLeft;
				walkingLeft.speed = 0.2f;
			}

			// go right
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				if (!onAir) {
					pBody->body->ApplyForceToCenter({ 60,0 }, true);
				}
				else {
					pBody->body->ApplyForceToCenter({ 30,0 }, true);
				}
				currentAnimation = &walkingRight;
				walkingRight.speed = 0.2f;
			}


			if (velocity.x < -maxSpeed) {
				velocity.x = -maxSpeed;
			}
			if (velocity.x > maxSpeed) {
				velocity.x = maxSpeed;
			}


			if (onAir) {
				if (previousAnimation == &idleRight ||
					previousAnimation == &walkingRight ||
					previousAnimation == &jumpingRight) {
					currentAnimation = &jumpingRight;
				}
				else if (previousAnimation == &idleLeft ||
					previousAnimation == &walkingLeft ||
					previousAnimation == &jumpingLeft) {
					currentAnimation = &jumpingLeft;
				}
			}
		}
		else {
			velocity = { 0, 0 };

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				velocity = { velocity.x, -maxSpeed };
			}

			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				velocity = { velocity.x, maxSpeed };
			}

			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				velocity = { -maxSpeed, velocity.y };
			}

			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				velocity = { maxSpeed, velocity.y };
			}
		}
	}
	else {
		currentAnimation = &death;
	}
	

	pBody->body->SetLinearVelocity(velocity);

	currentFrame = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	previousAnimation = currentAnimation;

	pBody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x + 1, position.y + 1, &currentFrame);

	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);
	texture = nullptr;
	texturePath = nullptr;

	idleRight.FullReset();
	walkingRight.FullReset();
	jumpingRight.FullReset();
	idleLeft.FullReset();
	walkingLeft.FullReset();
	jumpingLeft.FullReset();

	death.FullReset();

	app->physics->world->DestroyBody(pBody->body);

	active = false;

	return true;
}

void Player::OnCollision(PhysBody* otherBody)
{
	if (otherBody->typeTerrain == FLOOR) {
		onAir = false;
		canDoubleJump = true;
		groundPounding = false;
	}
	if (otherBody->typeTerrain == WALL) {
		//groundPounding = false;
	}
	if (otherBody->typeTerrain == DEATH) {
		Die();
	}
}

void Player::EndCollision(PhysBody* otherBody)
{
	if (otherBody->typeTerrain == FLOOR) {
		onAir = true;
	}
}

void Player::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pBody->body->SetTransform(position, 0);
}

void Player::Die()
{
	if (!godMode) {
		isAlive = false;
	}
}

void Player::GodSwitch() {
	if (!godMode) {
		godMode = true;
		pBody->body->SetGravityScale(0);
	}
	else {
		godMode = false;
		pBody->body->SetGravityScale(1);
	}
}
