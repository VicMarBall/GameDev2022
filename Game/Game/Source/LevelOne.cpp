#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "LevelOne.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

LevelOne::LevelOne(bool CurrentState) : Module(CurrentState)
{
	name.Create("level_one");
}

// Destructor
LevelOne::~LevelOne()
{}

// Called before render is available
bool LevelOne::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	//app->entityManager->Enable();

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->active = false;
	player->parameters = config.child("player");

	goal = (Goal*)app->entityManager->CreateEntity(EntityType::GOAL);
	goal->active = false;
	goal->parameters = config.child("goal");

	camX = config.child("camera").attribute("x").as_int();
	camY = config.child("camera").attribute("y").as_int();

	musicPath = (char*)config.child("audio").attribute("musicpath").as_string();

	return ret;
}

// Called before the first frame
bool LevelOne::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	// L03: DONE: Load map
	app->map->Load();
	app->audio->PlayMusic(musicPath, 1.0f);

	player->active = true;
	player->Start();

	goal->active = true;
	goal->Start();

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	app->render->camera.x = camX;
	app->render->camera.y = camY;

	return true;
}

// Called each loop iteration
bool LevelOne::PreUpdate()
{
	for (b2Body* b = app->physics->world->GetBodyList(); b; b = b->GetNext())
	{
		PhysBody* pB = (PhysBody*)b->GetUserData();
		if (pB->typeTerrain == FLOATING) {
			int posX = 0;
			int posY = 0;
			pB->GetPosition(posX, posY);
			if (posY < player->position.y + 32) {
				pB->body->SetActive(false);
			}
			else {
				pB->body->SetActive(true);
			}
		}
	}

	return true;
}

// Called each loop iteration
bool LevelOne::Update(float dt)
{
	if (player->Living() == false) {

		app->fade->FadeToBlack(this, (Module*)app->death_screen, 100);
	}

	if (player->Won() == true) {
		app->fade->FadeToBlack(this, (Module*)app->level_transition, 100);
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->level_one, 30);
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {

	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->level_one, 30);
	}

	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		if (app->render->DebugCamera == false)
			app->render->DebugCamera = true;
		else
			app->render->DebugCamera = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		player->GodSwitch();
	}
		

	if (app->render->DebugCamera == true) {
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	}

	else {
		if (player->position.x < app->win->GetWidth()/2 - camX) {
			app->render->camera.x = camX;
		}
		else if (player->position.x < app->map->mapData.width*app->map->mapData.tileWidth - app->win->GetWidth() / 2){
			app->render->camera.x = -player->position.x + app->render->camera.w / 2;
		}
		if (-player->position.y + app->render->camera.h / 2 < 0 && -player->position.y + app->render->camera.h / 2 > -app->map->mapData.height* app->map->mapData.tileHeight + app->win->GetHeight())
		app->render->camera.y = -player->position.y + app->win->GetHeight() / 2;
	}

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// Draw map
	app->map->Draw();

	return true;
}

// Called each loop iteration
bool LevelOne::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// L03: DONE 6: Implement a method to load the state load players's x and y
bool LevelOne::LoadState(pugi::xml_node& data)
{
	player->SetPosition(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());

	return true;
}

// L03: DONE 8: Create a method to save the state of the player
// using append_child and append_attribute
bool LevelOne::SaveState(pugi::xml_node& data)
{
	pugi::xml_node play = data.append_child("player");

	play.append_attribute("x") = player->position.x + 16;
	play.append_attribute("y") = player->position.y + 16;

	return true;
}

// Called before quitting
bool LevelOne::CleanUp()
{
	LOG("Freeing scene");
	player->CleanUp();
	app->tex->UnLoad(img);

	return true;
}
