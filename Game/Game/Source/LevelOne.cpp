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
#include "PathFinding.h"
#include "DeathScreen.h"
#include "GuiManager.h"

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


	//L02: DONE 3: Instantiate the player using the entity manager

	playerParameters = config.child("player");

	goalParameters = config.child("goal");

	for (pugi::xml_node CheckpointNode = config.child("checkpoint"); CheckpointNode; CheckpointNode = CheckpointNode.next_sibling("checkpoint")) {

		checkpointParameters[checkpointCount] = CheckpointNode;
		checkpointCount++;
	}


	for (pugi::xml_node EnemyNode = config.child("enemy"); EnemyNode; EnemyNode = EnemyNode.next_sibling("enemy")) {

		enemyParameters[enemyCount] = EnemyNode;
		enemyCount++;
	}

	for (pugi::xml_node CarrotNode = config.child("carrot"); CarrotNode; CarrotNode = CarrotNode.next_sibling("carrot")) {

		carrotsParameters[carrotsCount] = CarrotNode;
		carrotsCount++;
	}
	
	for (pugi::xml_node LivesNode = config.child("extralife"); LivesNode; LivesNode = LivesNode.next_sibling("extralife")) {

		extraLivesParameters[extraLivesCount] = LivesNode;
		extraLivesCount++;
	}

	camX = config.child("camera").attribute("x").as_int();
	camY = config.child("camera").attribute("y").as_int();

	musicPath = (char*)config.child("audio").attribute("musicpath").as_string();

	mapFileName = config.child("mapfile").attribute("path").as_string();

	return ret;
}

// Called before the first frame
bool LevelOne::Start()
{
	timer = 0;

	goToTitle = false;

	toExit = false;

	toResume = false;

	pause = false;

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
	player->CanShoot(false);
	player->Start();

	goal = (Goal*)app->entityManager->CreateEntity(EntityType::GOAL);
	goal->active = true;
	goal->parameters = goalParameters;
	goal->Start();

	for (int i = 0; i < checkpointCount; ++i) {
		checkpoint[i] = (Checkpoint*)app->entityManager->CreateEntity(EntityType::CHECKPOINT);
		checkpoint[i]->active = true;
		checkpoint[i]->parameters = checkpointParameters[i];
		checkpoint[i]->Start();
	}
	

	for (int i = 0; i < enemyCount; ++i) {
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

	carrotsPicked = 0;

	for (int i = 0; i < carrotsCount; ++i) {
		carrots[i] = (Carrot*)app->entityManager->CreateEntity(EntityType::CARROT);
		carrots[i]->active = true;
		carrots[i]->parameters = carrotsParameters[i];
		carrots[i]->Start();
	}

	for (int i = 0; i < extraLivesCount; ++i) {
		extraLives[i] = (ExtraLife*)app->entityManager->CreateEntity(EntityType::EXTRALIFE);
		extraLives[i]->active = true;
		extraLives[i]->parameters = extraLivesParameters[i];
		extraLives[i]->Start();
	}

	app->render->camera.x = camX;
	app->render->camera.y = camY;

	app->death_screen->level = 1;

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/path.png");
	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/x.png");

	lifeTexture = app->tex->Load("Assets/Textures/heart.png");

	// ui
	for (int i = 0; i < MAX_LIVESDRAWN; ++i) {
		livesUI[i] = (GuiImage*)app->guiManager->CreateGuiControl(GuiControlType::IMAGE, 0, NULL, {32 + (80 * i), 32, 64, 64}, this);
		livesUI[i]->SetTexture(lifeTexture);
	}

	carrotsCollectedText = (GuiText*)app->guiManager->CreateGuiControl(GuiControlType::TEXT, 0, "carrots?!", {600, 32, 32, 32}, this);

	resumeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "RESUME", { 100, 100, 75, 25 }, this);
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "SETTINGS", { 100, 150, 75, 25 }, this);
	backToTitleButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "BACK TO TITLE", { 100, 200, 75, 25 }, this);
	exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "EXIT", { 100, 250, 75, 25 }, this);

	resumeButton->TurnOFF();
	settingsButton->TurnOFF();
	backToTitleButton->TurnOFF();
	exitButton->TurnOFF();

	soundButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "sound", { 100, 250, 50, 25 }, this);
	soundButton->TurnOFF();

	backFromSettingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "BACK", { 100, 350, 50, 25 }, this);
	backFromSettingsButton->TurnOFF();

	timerText = (GuiText*)app->guiManager->CreateGuiControl(GuiControlType::TEXT, 0, "time", { 300, 20, 30, 20 }, this);

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
	if (!pause) {
		timer += dt * 0.001f;
	}

	if (goToTitle) {
		app->fade->FadeToBlack(this, (Module*)app->scene_title);
	}

	// pause
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		pause = !pause;

		if (pause) {
			player->PauseMovement();
			for (int i = 0; i < enemyCount; ++i) {
				if (enemy[i] != nullptr) {
					enemy[i]->PauseMovement();
				}
			}
			resumeButton->TurnON();
			settingsButton->TurnON();
			backToTitleButton->TurnON();
			exitButton->TurnON();
		}
		else {
			toResume = true;
		}
	}

	if (toResume) {
		player->ResumeMovement();
		for (int i = 0; i < enemyCount; ++i) {
			if (enemy[i] != nullptr) {
				enemy[i]->ResumeMovement();
			}
		}

		resumeButton->TurnOFF();
		settingsButton->TurnOFF();
		backToTitleButton->TurnOFF();
		exitButton->TurnOFF();

		toResume = false;
	}

	if (player->Living() == false) {

		app->fade->FadeToBlack(this, (Module*)app->death_screen, 100);
	}

	if (player->Won() == true) {
		app->fade->FadeToBlack(this, (Module*)app->level_transition, 100);
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
			else {
				enemy[i]->SetDefaultObjective();
				app->pathfinding->CreatePath(app->map->WorldToMap(enemy[i]->position.x + 8, enemy[i]->position.y + 8),
					app->map->WorldToMap(enemy[i]->GetObjective().x, enemy[i]->GetObjective().y));

				enemy[i]->SetPath(app->pathfinding->GetLastPath());
			}
		}
	}

	for (int i = 0; i < carrotsCount; i++) {
		if (carrots != nullptr) {
			if (carrots[i]->CheckPickingCarrot()) {
				carrotsPicked++;
			}
		}
	}

	for (int i = 0; i < extraLivesCount; i++) {
		if (extraLives != nullptr) {
			if (extraLives[i]->CheckPickingLife()) {
				player->AddLife();
			}
		}
	}
	if (app->moveInCheckpoints()) {
		for (int i = 0; i < checkpointCount; i++) {
			if (checkpoint[i]->CheckContact()) {
				if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
					int j = i + 1;
					if (j = checkpointCount) {
						j = 0;
					}
					player->SetPosition(checkpoint[j]->position.x, checkpoint[j]->position.y);
				}
				if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
					int j = i + 1;
					if (j = checkpointCount) {
						j = 0;
					}
					player->SetPosition(checkpoint[j]->position.x, checkpoint[j]->position.y);
				}
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
		app->fade->FadeToBlack(this, (Module*)app->level_one, 30);
	}

	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
		if (app->render->DebugCamera == false)
			app->render->DebugCamera = true;
		else
			app->render->DebugCamera = false;
	}

	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		app->checkpointState();

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		if (app->guiManager->debug) {
			app->guiManager->debug = false;
		}
		else {
			app->guiManager->debug = true;
		}
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

	for (int i = 0; i < player->GetRemainingLives(); ++i) {
		livesUI[i]->toDraw = true;
	}

	for (int i = player->GetRemainingLives(); i < MAX_LIVESDRAWN; ++i) {
		livesUI[i]->toDraw = false;
	}
	
	std::string stringCarrotsPicked = std::to_string(carrotsPicked);
	carrotsCollectedText->SetText(stringCarrotsPicked.c_str());
	carrotsCollectedText->AdaptWidth(20, stringCarrotsPicked.length());

	std::string stringTimer = std::to_string((int)timer);
	timerText->SetText(stringTimer.c_str());
	timerText->AdaptWidth(10, stringTimer.length());

	return true;
}

// Called each loop iteration
bool LevelOne::PostUpdate()
{
	bool ret = true;

	app->guiManager->Draw();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || toExit)
		ret = false;

	return ret;
}

// L03: DONE 6: Implement a method to load the state load players's x and y
bool LevelOne::LoadState(pugi::xml_node& data)
{
	if (data.child("state").attribute("state").as_bool()) {

		if (!active) {
			if (app->fromTitle) {
				app->fromTitle = false;
				app->fade->FadeToBlack((Module*)app->scene_title, this, 0);
				app->LoadGameRequest();
			}
			return true;
		}
		player->SetPosition(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());
		player->SetLives(data.child("player").attribute("lives").as_int());

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

		i = 0;
		for (pugi::xml_node CheckpointNode = data.child("checkpoints").child("checkpoint"); CheckpointNode; CheckpointNode = CheckpointNode.next_sibling("checkpoint")) {
			if (CheckpointNode.attribute("active").as_bool()) {
				if (checkpoint[i]->active) {
					checkpoint[i]->isPicked = true;
				}
			}
			else {
				if (checkpoint[i]->active) {
					checkpoint[i]->isPicked = false;
				}
			}
			i++;
		}

		i = 0;
		for (pugi::xml_node CarrotNode = data.child("carrots").child("carrot"); CarrotNode; CarrotNode = CarrotNode.next_sibling("carrot")) {
			if (CarrotNode.attribute("active").as_bool()) {
				if (!carrots[i]->active) {

					carrots[i] = (Carrot*)app->entityManager->CreateEntity(EntityType::CARROT);


					carrots[i]->active = true;
					carrots[i]->parameters = carrotsParameters[i];
					carrots[i]->Start();
				}
			}
			else {
				if (carrots[i]->active) {
					carrots[i]->isPicked = true;
				}
			}
			i++;
		}
		carrotsPicked = data.child("carrots").attribute("picked").as_int();

		i = 0;
		for (pugi::xml_node ExtraLifeNode = data.child("lives").child("extralife"); ExtraLifeNode; ExtraLifeNode = ExtraLifeNode.next_sibling("extralife")) {
			if (ExtraLifeNode.attribute("active").as_bool()) {
				if (!extraLives[i]->active) {
					extraLives[i] = (ExtraLife*)app->entityManager->CreateEntity(EntityType::EXTRALIFE);
					extraLives[i]->active = true;
					extraLives[i]->parameters = extraLivesParameters[i];
					extraLives[i]->Start();
				}
			
			}
			else {
				if (extraLives[i]->active) {
					extraLives[i]->isPicked;
				}
			}
			i++;
		}

		timer = data.child("timer").attribute("time_spent").as_float();
	}
	else {
		if (active) {
			app->fade->FadeToBlack(this, (Module*)app->level_two, 0);
			app->LoadGameRequest();
		}
	}
	return true;
}

// L03: DONE 8: Create a method to save the state of the player
// using append_child and append_attribute
bool LevelOne::SaveState(pugi::xml_node& data)
{
	pugi::xml_node state = data.append_child("state");

	if (active) {
		state.append_attribute("state") = true;

		pugi::xml_node play = data.append_child("player");

		play.append_attribute("x") = player->position.x + 16;
		play.append_attribute("y") = player->position.y + 16;
		play.append_attribute("lives") = player->GetRemainingLives();


		pugi::xml_node enem = data.append_child("enemies");
		for (int i = 0; i < enemyCount; i++) {
			pugi::xml_node curEnem = enem.append_child("enemy");
			if (enemy[i]->active) {
				curEnem.append_attribute("x") = enemy[i]->position.x;
				curEnem.append_attribute("y") = enemy[i]->position.y;
			}

			curEnem.append_attribute("active") = enemy[i]->active;
		}

		pugi::xml_node carr = data.append_child("carrots");
		for (int i = 0; i < carrotsCount; i++) {
			pugi::xml_node curCarr = carr.append_child("carrot");
			curCarr.append_attribute("active") = carrots[i]->active;
		}
		carr.append_attribute("picked") = carrotsPicked;

		pugi::xml_node eLives = data.append_child("lives");
		for (int i = 0; i < extraLivesCount; i++) {
			pugi::xml_node curLive = eLives.append_child("extralife");
			curLive.append_attribute("active") = extraLives[i]->active;
		}

		pugi::xml_node checks = data.append_child("checkpoints");
		for (int i = 0; i < checkpointCount; i++) {
			pugi::xml_node curCheck = checks.append_child("checkpoint");


			curCheck.append_attribute("active") = checkpoint[i]->CheckPicking();
		}

		pugi::xml_node timing = data.append_child("timer");
		timing.append_attribute("time_spent") = timer;

	}
	else {
		state.append_attribute("state") = false;
	}

	return true;
}

// Called before quitting
bool LevelOne::CleanUp()
{
	LOG("Freeing scene");
	app->entityManager->DestroyAllActiveEntities();

	app->guiManager->Clear(carrotsCollectedText);
	for (int i = 0; i < MAX_LIVESDRAWN; ++i) {
		app->guiManager->Clear(livesUI[i]);
	}

	app->guiManager->Clear(resumeButton);
	app->guiManager->Clear(settingsButton);
	app->guiManager->Clear(backToTitleButton);
	app->guiManager->Clear(exitButton);

	app->guiManager->Clear(soundButton);
	app->guiManager->Clear(backFromSettingsButton);

	app->guiManager->Clear(timerText);

	app->map->UnLoad();
	app->tex->UnLoad(img);
	app->tex->UnLoad(mouseTileTex);
	app->tex->UnLoad(originTex);

	return true;
}

bool LevelOne::OnGuiMouseClickEvent(GuiControl* control)
{
	// 1 == PLAY
	// 2 == CONTINUE
	// 3 == SETTINGS
	// 4 == CREDITS
	// 5 == EXIT

	switch (control->id)
	{
	case 1:
		pause = false;
		toResume = true;

		break;
	case 2:


		resumeButton->TurnOFF();
		settingsButton->TurnOFF();
		backToTitleButton->TurnOFF();
		exitButton->TurnOFF();

		soundButton->TurnON();
		backFromSettingsButton->TurnON();
		break;
	case 3: 
		goToTitle = true;
		break;
	case 4:
		toExit = true;
		break;
	case 5:

		break;
	case 6:
		resumeButton->TurnON();
		settingsButton->TurnON();
		backToTitleButton->TurnON();
		exitButton->TurnON();

		soundButton->TurnOFF();
		backFromSettingsButton->TurnOFF();
		break;
	default:
		break;
	}

	return false;
}