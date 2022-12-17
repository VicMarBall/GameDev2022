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

	winning.PushBack({ 64, 32, 32, 32 });

	// L07 TODO 5: Add physics to the player - initialize physics body
	pBody = app->physics->CreateRectangle(position.x, position.y, 32, 32, DYNAMIC);
	pBody->body->SetFixedRotation(true);
	pBody->listener = app->entityManager;
	pBody->entity = this;

	isAlive = true;

	win = false;

	facing = RIGHT;

	previousAnimation = &idleRight;

	onAir = true;

	maxSpeed = 6;

	godMode = false;

	for (int i = 0; i < 10; ++i)
	{
		storedBullets.Add((Bullet*)app->entityManager->CreateEntity(EntityType::BULLET));
	}

	return true;
}

bool Player::Update()
{
	// L07 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = pBody->body->GetLinearVelocity();
	Animation* currentAnimation = previousAnimation;
	SDL_Rect currentFrame = currentAnimation->GetCurrentFrame();

	if (isAlive && !win) {
		if (facing == RIGHT) {
			currentAnimation = &idleRight;
		}
		else {
			currentAnimation = &idleLeft;
		}

		// shooting
		if (canShoot) {
			if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
				for (ListItem<Bullet*>* bullet = storedBullets.start; bullet != NULL; bullet = bullet->next) {
					if (bullet->data->IsAvailable() == true) {
						switch (facing)
						{
						case Player::LEFT:
							bullet->data->SetBullet(Bullet::DIRECTIONS::LEFT, { position.x, position.y + 16 });
							pBody->body->ApplyForceToCenter({ 120, 0 }, true);
							break;
						case Player::RIGHT:
							bullet->data->SetBullet(Bullet::DIRECTIONS::RIGHT, { position.x + 32, position.y + 16 });
							pBody->body->ApplyForceToCenter({ -120, 0 }, true);
							break;
						default:
							break;
						}
						break;
					}
				}
			}
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

			if (currentAnimation == &idleLeft && velocity.x < 0) {
				currentAnimation = &walkingLeft;
				walkingLeft.speed = abs(velocity.x) * 0.03f;
				facing = LEFT;
			}

			if (currentAnimation == &idleRight && velocity.x > 0) {
				currentAnimation = &walkingRight;
				walkingRight.speed = abs(velocity.x) * 0.03f;
				facing = RIGHT;
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
				facing = LEFT;
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
				facing = RIGHT;
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
					facing = RIGHT;
				}
				else if (previousAnimation == &idleLeft ||
					previousAnimation == &walkingLeft ||
					previousAnimation == &jumpingLeft) {
					currentAnimation = &jumpingLeft;
					facing = LEFT;
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
	else if (!isAlive) {
		currentAnimation = &death;
	}
	else {
		currentAnimation = &winning;
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

	for (ListItem<Bullet*>* b = storedBullets.end; b != NULL; b = b->prev) {
		app->entityManager->DestroyEntity(b->data);
	}

	storedBullets.Clear();

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

	if (pBody != nullptr) {
		app->physics->DeleteBody(pBody);
	}

	

	active = false;

	return true;
}

void Player::OnCollision(PhysBody* otherBody)
{
	if (otherBody->typeTerrain == FLOOR || otherBody->typeTerrain == FLOATING) {
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
	if (otherBody->entity != nullptr) {
		if (otherBody->entity->type == EntityType::GOAL) {
			win = true;
			// win
			LOG("WIN");
		}

		if (otherBody->entity->type == EntityType::WALKINGENEMY || otherBody->entity->type == EntityType::FLYINGENEMY) {
			Die();
		}
	}
}

void Player::EndCollision(PhysBody* otherBody)
{
	if (otherBody->typeTerrain == FLOOR || otherBody->typeTerrain == FLOATING) {
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

void Player::CanShoot(bool can)
{
	canShoot = can;
}
