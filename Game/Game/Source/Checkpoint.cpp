#include "Checkpoint.h"
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

Checkpoint::Checkpoint() : Entity(EntityType::CHECKPOINT)
{
	name.Create("checkpoint");
}

Checkpoint::~Checkpoint() {}

bool Checkpoint::Awake() {

	return true;
}

bool Checkpoint::Start() {

	//initilize textures
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);

	// L07 TODO 4: Add a physics to an item - initialize the physics body
	pBody = app->physics->CreateRectangleSensor(position.x, position.y, 16, 48, STATIC);
	pBody->body->SetFixedRotation(true);
	pBody->body->SetActive(true);
	pBody->listener = app->entityManager;
	pBody->entity = this;

	return true;
}

bool Checkpoint::Update()
{
	// L07 TODO 4: Add a physics to an item - update the position of the object from the physics.
	pBody->GetPosition(position.x, position.y);
	SDL_Rect sect;
	sect.y = 0;
	sect.w = 32;
	sect.h = 48;
	if (CheckPicking()) {
		sect.x = 32;
		app->render->DrawTexture(texture, position.x + 8, position.y + 16, &sect);
	}
	else {
		sect.x = 0;
		app->render->DrawTexture(texture, position.x + 8, position.y + 16, &sect);
	}
	
	return true;
}

bool Checkpoint::CleanUp()
{
	return true;
}

void Checkpoint::OnCollision(PhysBody* otherBody)
{
	if (otherBody->entity != nullptr) {
		if (otherBody->entity->type == EntityType::PLAYER) {
			isPicked = true;
			inContact = true;
			app->lastCheckpoint = id;
			app->SaveGameRequest();
			LOG("Checkpoint :D");
		}
	}
}

void Checkpoint::EndCollision(PhysBody* otherBody)
{
	if (otherBody->entity != nullptr) {
		if (otherBody->entity->type == EntityType::PLAYER) {
			inContact = false;
		}
	}
}

bool Checkpoint::CheckPicking()
{
	if (isPicked) {
		return true;
	}
	return false;
}

bool Checkpoint::CheckContact()
{
	return inContact;
}
