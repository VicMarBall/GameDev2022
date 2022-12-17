#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "LevelTwo.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "PathFinding.h"
#include "DeathScreen.h"

#include "Defs.h"
#include "Log.h"

LevelTwo::LevelTwo(bool CurrentState) : Module(CurrentState)
{
	name.Create("level_two");
}

// Destructor
LevelTwo::~LevelTwo()
{}

// Called before render is available
bool LevelTwo::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	//app->entityManager->Enable();

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access

	//L02: DONE 3: Instantiate the player using the entity manager
	playerParameters = config.child("player");

	goalParameters = config.child("goal");

	for (pugi::xml_node EnemyNode = config.child("enemy"); EnemyNode; EnemyNode = EnemyNode.next_sibling("enemy")) {

		enemyParameters[enemyCount] = EnemyNode;
		enemyCount++;
	}

	camX = config.child("camera").attribute("x").as_int();
	camY = config.child("camera").attribute("y").as_int();

	musicPath = (char*)config.child("audio").attribute("musicpath").as_string();

	mapFileName = config.child("mapfile").attribute("path").as_string();

	return ret;
}

// Called before the first frame
bool LevelTwo::Start()
{
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

	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->active = true;
	player->parameters = playerParameters;
	player->CanShoot(true);
	player->Start();

	goal = (Goal*)app->entityManager->CreateEntity(EntityType::GOAL);
	goal->active = true;
	goal->parameters = goalParameters;
	goal->Start();


	for (int i = 0; i < enemyCount; i++) {
		if (enemyParameters[i].attribute("type").as_int() == 0) {
			enemy[i] = (Enemy*)app->entityManager->CreateEntity(EntityType::FLYINGENEMY);
		}
		else {
			enemy[i] = (Enemy*)app->entityManager->CreateEntity(EntityType::WALKINGENEMY);
		}
		enemy[i]->active = true;
		enemy[i]->parameters = enemyParameters[i];
		enemy[i]->Start();
	}

	app->render->camera.x = camX;
	app->render->camera.y = camY;

	app->death_screen->level = 2;

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/path.png");
	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/x.png");

	return true;
}

// Called each loop iteration
bool LevelTwo::PreUpdate()
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
bool LevelTwo::Update(float dt)
{
	if (player->Living() == false) {

		app->fade->FadeToBlack(this, (Module*)app->death_screen, 100);
	}

	if (player->Won() == true) {
		app->fade->FadeToBlack(this, (Module*)app->win_screen, 100);
	}

	for (int i = 0; i < enemyCount; i++) {
		if (enemy != nullptr) {
			iPoint playerCenter;
			playerCenter.Create(player->position.x + 16, player->position.y + 16);
			if (enemy[i]->IsInRadius(playerCenter)) {
				enemy[i]->SetObjective(playerCenter);
				app->pathfinding->CreatePath(app->map->WorldToMap(enemy[i]->position.x + 8, enemy[i]->position.y + 8),
					app->map->WorldToMap(enemy[i]->GetObjective().x, enemy[i]->GetObjective().y));

				enemy[i]->SetPath(app->pathfinding->GetLastPath());
			}
		}
	}


	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->level_one, 30);
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->level_two, 30);
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		app->fade->FadeToBlack(this, (Module*)app->level_two, 30);
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
		if (player->position.x < app->win->GetWidth() / 2 - camX) {
			app->render->camera.x = camX;
		}
		else if (player->position.x < app->map->mapData.width * app->map->mapData.tileWidth - app->win->GetWidth() / 2) {
			app->render->camera.x = -player->position.x + app->render->camera.w / 2;
		}
		if (-player->position.y + app->render->camera.h / 2 < 0 && -player->position.y + app->render->camera.h / 2 > -app->map->mapData.height * app->map->mapData.tileHeight + app->win->GetHeight())
			app->render->camera.y = -player->position.y + app->win->GetHeight() / 2;
	}

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
				const DynArray<iPoint>* p = app->pathfinding->GetLastPath();
				debugPath.Clear();
				for (int i = 0; i < p->Count(); ++i) {
					debugPath.PushBack(*p->At(i));
				}

				originSelected = false;
			}
			else
			{
				origin = mouseTile;
				originSelected = true;
				app->pathfinding->ClearLastPath();
			}
		}

		{
			// L12: Get the latest calculated path and draw
			const DynArray<iPoint>* path = &debugPath;
			app->pathfinding->DrawPath(path);
		}

		{
			// L12: Get the latest calculated path and draw
			for (int i = 0; i < enemyCount; i++) {
				const DynArray<iPoint>* path = enemy[i]->GetPath();
				app->pathfinding->DrawPath(path);
			}
		}

		// L12: Debug pathfinding
		iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
		app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
	}

	return true;
}

// Called each loop iteration
bool LevelTwo::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// L03: DONE 6: Implement a method to load the state load players's x and y
bool LevelTwo::LoadState(pugi::xml_node& data)
{
	if (data.child("state").attribute("state").as_bool()) {
		if (!active) {
			return true;
		}
		player->SetPosition(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());

		int i = 0;
		for (pugi::xml_node EnemyNode = data.child("enemies").child("enemy"); EnemyNode; EnemyNode = EnemyNode.next_sibling("enemy")) {
			if (EnemyNode.attribute("active").as_bool()) {
				if (!enemy[i]->active) {
					if (enemyParameters[i].attribute("type").as_int() == 0) {
						enemy[i] = (Enemy*)app->entityManager->CreateEntity(EntityType::FLYINGENEMY);
					}
					else {
						enemy[i] = (Enemy*)app->entityManager->CreateEntity(EntityType::WALKINGENEMY);
					}
					enemy[i]->active = true;
					enemy[i]->parameters = enemyParameters[i];
					enemy[i]->Start();
				}
				enemy[i]->SetPosition(EnemyNode.attribute("x").as_int(), EnemyNode.attribute("y").as_int());
			}
			else {
				if (enemy[i]->active) {
					enemy[i]->Die();
				}
			}
			i++;
		}
	}
	else {
		if (active) {
			app->fade->FadeToBlack(this, (Module*)app->level_one, 0);
			app->LoadGameRequest();
		}
	}
	return true;
}

// L03: DONE 8: Create a method to save the state of the player
// using append_child and append_attribute
bool LevelTwo::SaveState(pugi::xml_node& data)
{
	pugi::xml_node state = data.append_child("state");

	if (active) {
		state.append_attribute("state") = true;

		pugi::xml_node play = data.append_child("player");

		play.append_attribute("x") = player->position.x + 16;
		play.append_attribute("y") = player->position.y + 16;


		pugi::xml_node enem = data.append_child("enemies");
		for (int i = 0; i < enemyCount; i++) {
			pugi::xml_node curEnem = enem.append_child("enemy");
			if (enemy[i]->active) {
				curEnem.append_attribute("x") = enemy[i]->position.x;
				curEnem.append_attribute("y") = enemy[i]->position.y;
			}

			curEnem.append_attribute("active") = enemy[i]->active;
		}

	}
	else {
		state.append_attribute("state") = false;
	}
	return true;
}

// Called before quitting
bool LevelTwo::CleanUp()
{
	LOG("Freeing scene");
	app->entityManager->DestroyAllActiveEntities();
	//player->CleanUp();
	//enemy->CleanUp();
	//goal->CleanUp();
	app->map->UnLoad();
	app->tex->UnLoad(img);
	app->tex->UnLoad(mouseTileTex);
	app->tex->UnLoad(originTex);

	return true;
}
