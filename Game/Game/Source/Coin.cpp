#include "Coin.h"
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

Coin::Coin() : Entity(EntityType::COIN)
{
	name.Create("coin");
}

Coin::~Coin() {}

bool Coin::Awake() {

	return true;
}

bool Coin::Start() {

	//initilize textures
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);
	
	// L07 TODO 4: Add a physics to an item - initialize the physics body
	pBody = app->physics->CreateRectangleSensor(position.x, position.y, 32, 32, STATIC);
	pBody->body->SetFixedRotation(true);
	pBody->body->SetActive(true);
	pBody->listener = app->entityManager;
	pBody->entity = this;

	return true;
}

bool Coin::Update()
{
	// L07 TODO 4: Add a physics to an item - update the position of the object from the physics.
	pBody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x + 16, position.y + 16);

	return true;
}

bool Coin::CleanUp()
{
	return true;
}

void Coin::OnCollision(PhysBody* otherBody)
{
	if (otherBody->entity != nullptr) {
		if (otherBody->entity->type == EntityType::PLAYER) {
			isPicked = true;
			LOG("COIN PICKED :D");
		}
	}
}

bool Coin::CheckPickingCoin()
{
	if (active && isPicked) {
		active = false;
		return true;
	}
	return false;
}
