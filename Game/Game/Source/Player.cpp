#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
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
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();


	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 TODO 5: Add physics to the player - initialize physics body
	pBody = app->physics->CreateRectangle(position.x, position.y, 32, 32, DYNAMIC);
	pBody->body->SetFixedRotation(true);
	pBody->listener = (Module*)(app->entityManager);
	pBody->entity = this;

	onAir = true;

	return true;
}

bool Player::Update()
{
	// L07 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = pBody->body->GetLinearVelocity();
	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		//position.y -= 1;
		velocity.y = -10;
		
	}


	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		if (onAir) {
			velocity.y = 20;
			velocity.x = 0;
		}
	}
	

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		//position.x -= 1;
		velocity.x = -5;
	}


	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		//position.x += 1;
		velocity.x = 5;
	}

	pBody->body->SetLinearVelocity(velocity);


	pBody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision()
{
	onAir = false;
}
