#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "Timer.h"

#include "PugiXml/src/pugixml.hpp"
#include "Optick/include/optick.h"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class ModuleFadeToBlack;
class SceneIntro;
class SceneTitle;
class LevelOne;
class LevelTransition;
class LevelTwo;
class DeathScreen;
class WinScreen;
class EntityManager;
class Map;
class Physics;
class PathFinding;
class GuiManager;
//L07 TODO 2: Add Physics module

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// L03: DONE 1: Create methods to control that the real Load and Save happens at the end of the frame
	void LoadGameRequest();
	void SaveGameRequest();
	bool LoadFileExists();
	bool LoadFromFile();
	bool SaveToFile() ;
	bool moveInCheckpoints();
	void checkpointState();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	ModuleFadeToBlack* fade;
	SceneIntro* scene_intro;
	SceneTitle* scene_title;
	LevelOne* level_one;
	LevelTransition* level_transition;
	LevelTwo* level_two;
	DeathScreen* death_screen;
	WinScreen* win_screen;
	EntityManager* entityManager;
	Map* map;
	//L07 TODO 2: Add Physics module
	Physics* physics;
	PathFinding* pathfinding;
	GuiManager* guiManager;

	bool fromTitle;
	bool fromCheckpoint;
	int lastCheckpoint;

	bool vsync;

	float dt;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	// L01: DONE 2: Create new variables from pugui namespace:
	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;

	// L03: DONE 1: Create control variables to control that the real Load and Save happens at the end of the frame
    bool saveGameRequested;
	bool loadGameRequested;

	bool moveCheckpoints = false;


	// L13: TODO 4: Calculate some timing measures
// required variables are provided:
	Timer timer;
	//PerfTimer ptimer;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	float secondsSinceStartup = 0.0f;

	uint32 maxFrameDuration = 0;
};

extern App* app;

#endif	// __APP_H__