#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "LevelOne.h"
#include "LevelTwo.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "PathFinding.h"

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

	
	enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::WALKINGENEMY);
	enemy->active = false;
	enemy->parameters = config.child("enemy");
	
	/*/for (pugi::xml_node EnemyNode = config.child("enemy"); EnemyNode; EnemyNode = EnemyNode.next_sibling("enemy")) {
		
		Enemy* currentEnemy;
		if (EnemyNode.attribute("type").as_int()==1){
			currentEnemy = (Enemy*)app->entityManager->CreateEntity(EntityType::WALKINGENEMY);}
		else {
			currentEnemy = (Enemy*)app->entityManager->CreateEntity(EntityType::FLYINGENEMY);}
		currentEnemy->active = false;
		currentEnemy->parameters = EnemyNode;
		//enemy->Add(currentEnemy);
	//}*/

	camX = config.child("camera").attribute("x").as_int();
	camY = config.child("camera").attribute("y").as_int();

	musicPath = (char*)config.child("audio").attribute("musicpath").as_string();

	mapFileName = config.child("mapfile").attribute("path").as_string();

	return ret;
}

// Called before the first frame
bool LevelOne::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	// L03: DONE: Load map
	app->map->SetMapFileName(mapFileName);

	bool retLoad = app->map->Load();

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	app->audio->PlayMusic(musicPath, 1.0f);

	player->active = true;
	player->Start();

	goal->active = true;
	goal->Start();

	enemy->active = true;
	enemy->Start();

	/*for (ListItem<Enemy*>* currentEnemy = enemy->start; currentEnemy; currentEnemy = currentEnemy->next) {
		currentEnemy->data->active = true;
		currentEnemy->data->Start();
	}*/

	app->render->camera.x = camX;
	app->render->camera.y = camY;

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/path.png");
	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/x.png");

	return true;
}

// Called each loop iteration
bool LevelOne::PreUpdate()
{
	for (b2Body* b = app->physics->world->GetBodyList(); b; b = b->GetNext())
	{
		PhysBody* pB = (PhysBody*)b->GetUserData();
		if (pB != nullptr) {
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
		if (-player->position.y + app->render->camera.h / 2 < 0 && -player->position.y + app->render->camera.h / 2 > -app->map->mapData.height * app->map->mapData.tileHeight + app->win->GetHeight())
			app->render->camera.y = -player->position.y + app->win->GetHeight() / 2;
	}

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// Draw map
	app->map->Draw();

	if (app->physics->debug) {
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);
		iPoint mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,
			mouseY - app->render->camera.y);

		//Convert again the tile coordinates to world coordinates to render the texture of the tile
		iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
		app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

		//Test compute path function
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (originSelected == true)
			{
				app->pathfinding->CreatePath(origin, mouseTile);
				originSelected = false;
			}
			else
			{
				origin = mouseTile;
				originSelected = true;
				app->pathfinding->ClearLastPath();
			}
		}

		// L12: Get the latest calculated path and draw
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
		}

		// L12: Debug pathfinding
		iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
		app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
	}

	

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

	enemy->SetPosition(data.child("enemies").child("enemy").attribute("x").as_int(), data.child("enemies").child("enemy").attribute("y").as_int());

	return true;
}

// L03: DONE 8: Create a method to save the state of the player
// using append_child and append_attribute
bool LevelOne::SaveState(pugi::xml_node& data)
{
	pugi::xml_node play = data.append_child("player");

	play.append_attribute("x") = player->position.x + 16;
	play.append_attribute("y") = player->position.y + 16;

	pugi::xml_node enem = data.append_child("enemies");
	pugi::xml_node curEnem = enem.append_child("enemy");
	curEnem.append_attribute("x") = enemy->position.x;
	curEnem.append_attribute("y") = enemy->position.y;

	/*for (ListItem<Enemy*>* currentEnemy = enemy->start; currentEnemy->data; currentEnemy = currentEnemy->next) {
		pugi::xml_node curEnem = enem.append_child("enemy");
		curEnem.append_attribute()

	}*/

	return true;
}

// Called before quitting
bool LevelOne::CleanUp()
{
	LOG("Freeing scene");
	player->CleanUp();
	enemy->CleanUp();
	goal->CleanUp();
	app->map->UnLoad();
	app->tex->UnLoad(img);
	app->tex->UnLoad(mouseTileTex);
	app->tex->UnLoad(originTex);


	return true;
}
