#ifndef __LEVELONE_H__
#define __LEVELONE_H__

#include "Module.h"
#include "Player.h"
#include "Goal.h"
#include "Carrot.h"
#include "ExtraLife.h"
#include "Enemies.h"
#include "Checkpoint.h"
#include "List.h"

#include "GuiImage.h"
#include "GuiText.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckbox.h"

#define MAX_ENEMIES 20

#define MAX_CARROTS 20

#define MAX_EXTRALIVES 5

#define MAX_LIVESDRAWN 5

#define MAX_CHECKPOINTS 3

struct SDL_Texture;

class LevelOne : public Module
{
public:

	LevelOne(bool CurrentState);

	// Destructor
	virtual ~LevelOne();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	//L02: DONE 3: Declare a Player attribute 
	pugi::xml_node playerParameters;
	pugi::xml_node goalParameters;
	pugi::xml_node checkpointParameters[MAX_CHECKPOINTS];
	pugi::xml_node enemyParameters[MAX_ENEMIES];
	pugi::xml_node carrotsParameters[MAX_CARROTS];
	pugi::xml_node extraLivesParameters[MAX_EXTRALIVES];

	Player* player;
	Goal* goal;
	Checkpoint* checkpoint[MAX_CHECKPOINTS];
	Enemy* enemy[MAX_ENEMIES];
	Carrot* carrots[MAX_CARROTS];
	ExtraLife* extraLives[MAX_EXTRALIVES];



	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

private:
	SDL_Texture* img;
	char* musicPath;
	SString mapFileName;

	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	bool pause = false;

	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	int camX;
	int camY;

	DynArray<iPoint> debugPath;

	int enemyCount = 0;

	int carrotsCount = 0;

	int extraLivesCount = 0;

	int checkpointCount = 0;

	SDL_Texture* lifeTexture;

	GuiImage* livesUI[MAX_LIVESDRAWN];

	int carrotsPicked;
	GuiText* carrotsCollectedText;

	float timer;
	GuiText* timerText;


	SDL_Texture* longButtonTexture = nullptr;
	SDL_Texture* shortButtonTexture = nullptr;
	SDL_Texture* checkboxTexture = nullptr;
	SDL_Texture* sliderBoxTexture = nullptr;

	// pause ui
	GuiButton* resumeButton;
	GuiButton* settingsButton;
	GuiButton* backToTitleButton;
	GuiButton* exitButton;

	bool goToTitle;
	bool toExit;
	bool toResume;

	GuiButton* backFromSettingsButton;

	GuiSlider* musicVolumeSlider;
	GuiSlider* SFXVolumeSlider;

	GuiCheckbox* fullScreenCheckbox;
	GuiCheckbox* VSyncCheckbox;
};

#endif // __LEVELONE_H__